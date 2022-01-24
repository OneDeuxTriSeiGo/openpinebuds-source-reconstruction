/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dhcpd.h"
#include "lwip/tcpip.h"
#include "common_subr.h"
#include "lwip/api.h"
#include "dhcpd_conf.h"
#include "leases.h"
#include "lwip/dhcp.h"
#include "lwip/sockets.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip_init.h"
#include "hal_trace.h"

static struct netif *netif;

static struct udp_pcb *dhcpd_udp_pcb = NULL;
static struct dhcpOfferedAddr leases[DHCPD_MAX_LEASES];
static struct dhcpd payload_out;
static struct server_config_t server_config;

int pass_wan_domain = 0;
#define DHCP_SERVER_PORT 67

//pickup what i want according to "code" in the packet, "dest" callback
unsigned char *dhcpd_pickup_opt(struct dhcpd *packet, int code, int dest_len, void *dest)
{
    int i, length;
    unsigned char *ptr;
    int over = 0, done = 0, curr = OPTION_FIELD;
    unsigned char len;

    ptr = packet->options;
    i = 0;
    length = 308;
    while (!done) {
        if (i >= length) {
            //printf( "Option fields too long.");
            return 0;
        }

        if (ptr[i + OPT_CODE] == code) {
            if (i + 1 + ptr[i + OPT_LEN] >= length)
                return 0;

            if (dest) {
                len = ptr[i + OPT_LEN];
                if (len > dest_len) {
                    //printf( "Option fields too long to fit in dest.");
                    return 0;
                }

                memcpy(dest, &ptr[i + OPT_DATA], (int)len);
            }

            return &ptr[i + OPT_DATA];
        }

        switch (ptr[i + OPT_CODE]) {
        case DHCP_PADDING:
            i++;
            break;

        case DHCP_OPTION_OVER:
            if (i + 1 + ptr[i + OPT_LEN] >= length)
                return 0;

            over = ptr[i + 3];
            i += ptr[OPT_LEN] + 2;
            break;

        case DHCP_END:
            if (curr == OPTION_FIELD && over & FILE_FIELD) {
                ptr = packet->file;
                i = 0;
                length = 128;
                curr = FILE_FIELD;
            } else if (curr == FILE_FIELD && over & SNAME_FIELD) {
                ptr = packet->sname;
                i = 0;
                length = 64;
                curr = SNAME_FIELD;
            } else
                done = 1;
            break;

        default:
            i += ptr[OPT_LEN + i] + 2;
        }
    }

    return 0;
}

int dhcpd_add_option(unsigned char *ptr, unsigned char code, unsigned char len, void *data)
{
    int end;

    // Search DHCP_END
    end = 0;
    while (ptr[end] != DHCP_END) {
        if (ptr[end] == DHCP_PADDING)
            end++;
        else
            end += ptr[end + OPT_LEN] + 2; // 2 is opt_code and opt_len cost 2 bytes
    }

    if (end + len + 2 + 1 >= 308) {
        //printf( "Option 0x%02x cannot not fit into the packet!", code);
        return 0;
    }

    ptr += end; //rebuild pointer

    ptr[OPT_CODE] = code;
    ptr[OPT_LEN] = len; // bytes number of data stored in option
    memcpy(&ptr[OPT_DATA], data, len);

    // Reassign DHCP_END
    ptr += (len + 2);
    *ptr = DHCP_END;
    return (len + 2); // return this operation costs option bytes number
}

struct dhcpOfferedAddr *DHCPD_FindLeaseByYiaddr(struct in_addr yiaddr)
{
    unsigned int i;

    for (i = 0; i < server_config.max_leases; i++) {
        if (leases[i].yiaddr.s_addr == yiaddr.s_addr)
            return &leases[i];
    }
    return 0;
}

struct in_addr DHCPD_FindAddress()
{
    uint32_t addr;
    struct in_addr ret;
    struct dhcpOfferedAddr *lease = 0;
    for (addr = ntohl(server_config.start.s_addr);
         addr <= ntohl(server_config.end.s_addr);
         addr++) {
        // ie, xx.xx.xx.0 or xx.xx.xx.255 or itself
        if ((addr & 0xFF) == 0 ||
            (addr & 0xFF) == 0xFF ||
            (addr == ntohl(server_config.server.s_addr))) {
            continue;
        }
        ret.s_addr = ntohl(addr);
        if ((lease = DHCPD_FindLeaseByYiaddr(ret)) == 0) {
            return ret;
        }
    }
    ret.s_addr = 0;
    return ret;
}

struct dhcpOfferedAddr *DHCPD_FindLeaseByChaddr(uint8_t *chaddr)
{
    unsigned int i;

    for (i = 0; i < server_config.max_leases; i++) {
        if (memcmp(leases[i].chaddr, chaddr, 6) == 0)
            return &(leases[i]);
    }

    return NULL;
}

static void make_dhcpd_packet(struct dhcpd *packet, struct dhcpd *oldpacket, char type)
{
    uint32_t lease_time = server_config.lease;
    unsigned char *option = packet->options;
    char domain_name[255] = {0};
    lease_time = htonl(lease_time);
    memset(packet, 0, sizeof(struct dhcpd));
    packet->op = BOOTREPLY;
    packet->htype = ETH_10MB;
    packet->hlen = ETH_10MB_LEN;
    packet->xid = oldpacket->xid;
    memcpy(packet->chaddr, oldpacket->chaddr, 16);
    packet->flags = 0x0; // force to static
    packet->ciaddr = oldpacket->ciaddr;
    packet->yiaddr = oldpacket->yiaddr;
    packet->siaddr = server_config.siaddr.s_addr;
    packet->giaddr = oldpacket->giaddr;
    packet->cookie = htonl(DHCP_MAGIC);
    packet->options[0] = DHCP_END;
    memcpy(packet->sname, server_config.sname, 6);
    dhcpd_add_option(option, DHCP_MESSAGE_TYPE, sizeof(type), &type);
    dhcpd_add_option(option, DHCP_SERVER_ID, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
    dhcpd_add_option(option, DHCP_LEASE_TIME, sizeof(lease_time), &lease_time);
    dhcpd_add_option(option, DHCP_SUBNET, sizeof(server_config.mask.s_addr), &server_config.mask.s_addr);
    dhcpd_add_option(option, DHCP_ROUTER, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
    dhcpd_add_option(option, DHCP_DNS_SERVER, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
    memcpy(domain_name, DEFAULT_DOMAIN, sizeof(DEFAULT_DOMAIN));
    dhcpd_add_option(option, DHCP_DOMAIN_NAME, strlen(domain_name), domain_name);
}

static int discover(struct dhcpd *packetinfo)
{
    struct in_addr addr;
    int8_t i = 0;
    struct dhcpOfferedAddr *lease;
    if (memcmp(packetinfo->chaddr, "\x00\x00\x00\x00\x00\x00", 6) == 0 ||
        memcmp(packetinfo->chaddr, "\xff\xff\xff\xff\xff\xff", 6) == 0) {
        return -1;
    }
    if ((lease = DHCPD_FindLeaseByChaddr(packetinfo->chaddr)) == NULL) {
        addr = DHCPD_FindAddress();

        TRACE(0, "%s 0x%x.", __func__, addr.s_addr);

        while ((leases[i].yiaddr.s_addr) != 0)
            i++;

        TRACE(0, "%s i=%d..", __func__, i);
        MEMCPY(leases[i].chaddr, packetinfo->chaddr, 6);
        leases[i].yiaddr = addr;
        packetinfo->yiaddr = addr.s_addr;
        lease = &(leases[i]);
    }

    memset(&payload_out, 0, sizeof(struct dhcpd));
    make_dhcpd_packet(&payload_out, packetinfo, DHCPOFFER);
    payload_out.yiaddr = lease->yiaddr.s_addr;

    return 0;
}

static int request(struct dhcpd *packetinfo)
{
    uint32_t lease_time = server_config.lease;
    char type;
    unsigned char *option = payload_out.options;

    char domain_name[255] = {0};
    lease_time = htonl(lease_time);

    memset(&payload_out, 0, sizeof(struct dhcpd));
    struct dhcpOfferedAddr *lease;
    if ((lease = DHCPD_FindLeaseByChaddr(packetinfo->chaddr)) != NULL) {
        type = DHCPACK;
        payload_out.op = BOOTREPLY;
        payload_out.htype = ETH_10MB;
        payload_out.hlen = ETH_10MB_LEN;
        payload_out.xid = packetinfo->xid;
        memcpy(payload_out.chaddr, packetinfo->chaddr, 16);
        payload_out.flags = 0x0; // force to static
        payload_out.ciaddr = packetinfo->ciaddr;
        payload_out.siaddr = server_config.siaddr.s_addr;
        payload_out.giaddr = packetinfo->giaddr;
        payload_out.cookie = htonl(DHCP_MAGIC);
        payload_out.options[0] = DHCP_END;
        payload_out.yiaddr = lease->yiaddr.s_addr;
#ifdef HOST_CLIENT_IP
        extern void HalWifiHostSetClientIpAddr(uint8_t index, uint32_t ip);
        HalWifiHostSetClientIpAddr(0, payload_out.yiaddr);
#endif
        dhcpd_add_option(option, DHCP_MESSAGE_TYPE, sizeof(type), &type);
        dhcpd_add_option(option, DHCP_SERVER_ID, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
        dhcpd_add_option(option, DHCP_LEASE_TIME, sizeof(lease_time), &lease_time);
        dhcpd_add_option(option, DHCP_SUBNET, sizeof(server_config.mask.s_addr), &server_config.mask.s_addr);
        dhcpd_add_option(option, DHCP_ROUTER, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
        dhcpd_add_option(option, DHCP_DNS_SERVER, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
        memcpy(domain_name, DEFAULT_DOMAIN, sizeof(DEFAULT_DOMAIN));
        dhcpd_add_option(option, DHCP_DOMAIN_NAME, strlen(domain_name), domain_name);
    } else {
        type = DHCPNAK;
        payload_out.op = BOOTREPLY;
        payload_out.htype = ETH_10MB;
        payload_out.hlen = ETH_10MB_LEN;
        payload_out.xid = packetinfo->xid;
        memcpy(payload_out.chaddr, packetinfo->chaddr, 16);
        payload_out.flags = 0x0; // force to static
        payload_out.ciaddr = packetinfo->ciaddr;
        payload_out.giaddr = packetinfo->giaddr;
        payload_out.cookie = htonl(DHCP_MAGIC);
        payload_out.options[0] = DHCP_END;
        dhcpd_add_option(option, DHCP_MESSAGE_TYPE, sizeof(type), &type);
        dhcpd_add_option(option, DHCP_SERVER_ID, sizeof(server_config.server.s_addr), &server_config.server.s_addr);
    }

    return 0;
}

#define START_IP 0
#define END_IP 1
#define BOOT_IP 2
void ip_create_by_config(int flag, char *ipaddr)
{
    int i, j;
    i = j = 0;
    while (ipaddr[i] != '\0') {
        if (ipaddr[i] == '.')
            j++;
        i++;
        if (j >= 3)
            break;
    }
    ipaddr[i] = '\0';
    switch (flag) {
    case START_IP:
        strcat(ipaddr, "150");
        break;
    case END_IP:
        strcat(ipaddr, "200");
        break;
    case BOOT_IP:
        strcat(ipaddr, "5");
        break;
    default:
        break;
    }
    TRACE(0, "DHCP IP: [ %s ]\n\r", ipaddr);
}

static int init_config(void)
{
    u32_t start;
    u32_t end;
    u32_t sa = 0;
    int val;

    memset(&server_config, 0, sizeof(struct server_config_t));
    inet_aton(DEFAULT_SERVER_IP, (struct in_addr *)&sa);
    server_config.server.s_addr = sa;
    sa = 0;
    inet_aton(DEFAULT_MASK, (struct in_addr *)&sa);
    server_config.mask.s_addr = sa;
    inet_aton(DEFAULT_START, (struct in_addr *)&start);
    server_config.start.s_addr = start;
    inet_aton(DEFAULT_END, (struct in_addr *)&end);
    server_config.end.s_addr = end;

    server_config.max_leases = DHCPD_MAX_LEASES;
    // lease time
    val = DEFAULT_LEASE_TIME;  //3600
    server_config.lease = val; //3600

    server_config.conflict_time = DEFAULT_CONFLICT_TIME; //3600
    server_config.decline_time = DEFAULT_DECLINE_TIME;   //3600
    server_config.min_lease = DEFAULT_MIN_LEASE_TIME;    //60
    server_config.offer_time = DEFAULT_MIN_LEASE_TIME;   //60
    server_config.auto_time = DEFAULT_AUTO_TIME;         //3

    server_config.sname = DEFAULT_SNAME;
    server_config.boot_file = DEFAULT_BOOT_FILE;

    sa = 0;
    inet_aton(DEFAULT_BOOT_IP, (struct in_addr *)&sa);
    server_config.siaddr.s_addr = sa;

    return 0;
}

int dhcpd_init_config(struct server_config_t *config)
{
    u32_t start;
    u32_t end;
    u32_t sa = 0;
    int val;

    memset(&server_config, 0, sizeof(struct server_config_t));

    /* we use first listen interface as server IP */
    server_config.server.s_addr = config->server.s_addr;
    server_config.mask.s_addr = config->mask.s_addr;
    server_config.start.s_addr = config->start.s_addr;

    // end address
    server_config.end.s_addr = config->end.s_addr;
    server_config.max_leases = DHCPD_MAX_LEASES;

    // lease time
    val = DEFAULT_LEASE_TIME; //3600

    server_config.lease = val; //3600

    server_config.conflict_time = DEFAULT_CONFLICT_TIME; //3600
    server_config.decline_time = DEFAULT_DECLINE_TIME;   //3600
    server_config.min_lease = DEFAULT_MIN_LEASE_TIME;    //60
    server_config.offer_time = DEFAULT_MIN_LEASE_TIME;   //60
    server_config.auto_time = DEFAULT_AUTO_TIME;         //3

    server_config.sname = DEFAULT_SNAME;
    server_config.boot_file = DEFAULT_BOOT_FILE;

    sa = 0;
    inet_aton(DEFAULT_BOOT_IP, (struct in_addr *)&sa);
    server_config.siaddr.s_addr = sa;

    return 0;
}

int8_t dhcp_process(void *packet_addr)
{
    char state;

    if (dhcpd_pickup_opt((struct dhcpd *)packet_addr, DHCP_MESSAGE_TYPE, sizeof(state), &state) == NULL) {
        TRACE(0, "[DHCPD]: couldn't get option from packet, ignoring");
        return 0;
    }

    switch (state) {
    case DHCPDISCOVER:
        TRACE(0, "discover packet....\r\n");
        discover(packet_addr);
        break;

    case DHCPREQUEST:
        TRACE(0, "request packet...\n\r");
        request(packet_addr);
        break;

    default:
        TRACE(0, "unknown message\n\r");
    }
    return 1;
}

void UDP_Receive(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    ip_addr_t destAddr;
    destAddr.addr = htonl(IPADDR_BROADCAST);

    if (p != NULL) {
        if (dhcp_process(p->payload) != 0) {
            pbuf_free(p);

            p = pbuf_alloc(PBUF_TRANSPORT, sizeof(payload_out), PBUF_RAM);
            memcpy(p->payload, &payload_out, sizeof(payload_out));
            udp_sendto(upcb, p, &destAddr, port);
        }

        pbuf_free(p);
    }
}

void DHCPD_daemon_start(void)
{
    netif = GetNetif(NET_IF_AP);
    if (netif == NULL) {
        return;
    }
    TRACE(0, "%s %d.", __func__, __LINE__);
    memset(leases, 0, sizeof(struct dhcpOfferedAddr) * DHCPD_MAX_LEASES);
    init_config();

    netif_set_addr(netif, (ip_addr_t *)&server_config.server, (ip_addr_t *)&server_config.mask, (ip_addr_t *)&server_config.server);
    netif_set_up(netif);

    dhcpd_udp_pcb = udp_new();
    ip_set_option(dhcpd_udp_pcb, SOF_BROADCAST);
    udp_bind(dhcpd_udp_pcb, IP_ADDR_ANY, DHCP_SERVER_PORT);
    udp_recv(dhcpd_udp_pcb, UDP_Receive, NULL);
}

void DHCPD_daemon_stop(void)
{
    TRACE(0, "%s %d.", __func__, __LINE__);

    netif_set_down(netif);

    if (dhcpd_udp_pcb) {
        udp_remove(dhcpd_udp_pcb);
        dhcpd_udp_pcb = NULL;
    }
}
