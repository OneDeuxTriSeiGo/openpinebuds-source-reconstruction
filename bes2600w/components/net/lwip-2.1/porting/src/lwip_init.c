/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "lwip_init.h"
#include "lwip/sockets.h"
#include "lwip/if_api.h"
#include "lwip/netdb.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "cmsis_os2.h"
#include "bwifi_interface.h"
#include "hal_trace.h"

extern struct netif if_wifi;
extern struct netif if_wifi_ap;

static struct netif *g_p_netif = NULL;
static struct netif *g_p_netif_ap = NULL;


static osSemaphoreId_t dhcp_sem;

static void netif_status_callback(struct netif *netif)
{
    printf("=======%s:=============\r\n", __func__);
    if (netif_is_up(netif) && (!ip_addr_isany(&netif->ip_addr))) {
        printf("%s:\r\n", __func__);
        printf("ip      %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
        printf("netmask %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&netif->netmask));
        printf("gateway %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&netif->gw));

        // for ARP filter feature
        struct ip_info ipinfo;
        memcpy(&ipinfo.ip, &netif->ip_addr, 4);
        memcpy(&ipinfo.gw, &netif->gw, 4);
        memcpy(&ipinfo.netmask, &netif->netmask, 4);
        bwifi_set_ip_addr((netif == &if_wifi) ? WIFI_IF_STATION : WIFI_IF_SOFTAP, &ipinfo);
        WifiLinkedInfo *info = HalHmosGetWifiInfo();
        if (info) {
            uint32_t ip = ((ip4_addr_t *)&netif->ip_addr)->addr;
            info->ipAddress = ip;
            printf("%s set info->ipAddress:%u\n", __func__, ip);
        }
        osSemaphoreRelease(dhcp_sem);
    }
}

int GetDynamicIp(enum netifType intf)
{
    struct netif *netif = GetNetif(intf);
    if (netif == NULL) {
        return -1;
    }
    dhcp_sem = osSemaphoreNew(1, 0, NULL);
    if (!dhcp_sem) {
        printf("osSemaphoreNew error\r\n");
        return -1;
    }
#if LWIP_NETIF_STATUS_CALLBACK
    netif_set_status_callback(netif, netif_status_callback);
#elif LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback(netif, netif_status_callback);
#endif
    printf("%s dhcp start\n", __func__);
    dhcp_start(netif);
    if (osSemaphoreAcquire(dhcp_sem, 30 * 1000) != 0) {
        printf("dhcp timeout\r\n");
        osSemaphoreDelete(dhcp_sem);
        dhcp_stop(netif);
        return -1;
    }
    osSemaphoreDelete(dhcp_sem);
    dhcp_stop(netif);
    return 0;
}

int SetStaticIp(enum netifType intf, IpConfig *staticIp)
{
    if (staticIp == NULL) {
        return -1;
    }
    struct netif *netif = GetNetif(intf);
    if (netif == NULL) {
        return -1;
    }
    netif_set_addr(netif, (ip4_addr_t *)&staticIp->ipAddress,
                   (ip4_addr_t *)&staticIp->netmask, (ip4_addr_t *)&staticIp->gateway);

    printf("%s:\r\n", __func__);
    printf("ip      %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&staticIp->ipAddress));
    printf("netmask %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&staticIp->netmask));
    printf("gateway %s\r\n", ip4addr_ntoa((const ip4_addr_t *)&staticIp->gateway));

    bool dns_valid = false;
    for (int i = 0; i < WIFI_MAX_DNS_NUM; i++) {
        if (staticIp->dnsServers[i]) {
            dns_setserver(i, (const ip_addr_t *)&staticIp->dnsServers[i]);
            dns_valid = true;
            printf("dns[%d] %s\r\n", i, ip4addr_ntoa((const ip4_addr_t *)&staticIp->dnsServers[i]));
        }
    }
    if (!dns_valid) {
        ip4_addr_t dns;
        IP4_ADDR(&dns, 8, 8, 8, 8);
        dns_setserver(0, (const ip_addr_t *)&dns);
        printf("default dns %s\n", ip4addr_ntoa((const ip4_addr_t *)&dns));
    }

    struct ip_info ipinfo;
    memcpy(&ipinfo.ip, &netif->ip_addr, 4);
    memcpy(&ipinfo.gw, &netif->gw, 4);
    memcpy(&ipinfo.netmask, &netif->netmask, 4);
    bwifi_set_ip_addr(intf ? WIFI_IF_SOFTAP : WIFI_IF_STATION, &ipinfo);
    return 0;
}

void SetNetifStatus(enum netifType intf, bool netif_up, bool netif_default)
{
    struct netif *netif = GetNetif(intf);
    if (netif == NULL) {
        return;
    }
    if (netif_up) {
        netif_set_link_up(netif);
        netif_set_up(netif);
        if (netif_default) {
            netif_set_default(netif);
        }
    } else {
        netif_set_link_down(netif);
        netif_set_down(netif);
    }
}

extern void ethernetif_input(u16_t devnum, void *p_buf, int size);
extern err_t ethernetif_init(struct netif *netif);
extern int bwifi_get_own_mac(u8 *addr);
int InitNetif()
{
    uint8_t mac_addr[6] = {0};
    static bool tcpinitflag = false;
    if (tcpinitflag == false) {
        tcpinitflag = true;
        tcpip_init(NULL, NULL);
    }

    bwifi_reg_eth_input_handler(ethernetif_input);
    bwifi_get_own_mac(mac_addr);

    if (g_p_netif) {
        netif_remove(g_p_netif);
    }
    g_p_netif = &if_wifi;
    lwip_netif_mac_addr_init(g_p_netif, mac_addr, ETH_ALEN);

    if (netif_add(g_p_netif, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4, NULL, ethernetif_init, tcpip_input) == 0) {
        printf("netif add wifi interface failed\n");
        return -1;
    }
    netif_set_default(g_p_netif);
    netif_set_status_callback(g_p_netif, netif_status_callback);

#ifdef SOFTAP_ENABLE
    if (g_p_netif_ap) {
        netif_remove(g_p_netif_ap);
    }
    g_p_netif_ap = &if_wifi_ap;
    mac_addr[5] += 1;
    lwip_netif_mac_addr_init(g_p_netif_ap, mac_addr, ETH_ALEN);
    if (netif_add(g_p_netif_ap, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4, NULL, ethernetif_init, tcpip_input) == 0) {
        printf("netif add ap interface failed\n");
        return -1;
    }
#endif
    return 0;
}

struct netif *GetNetif(enum netifType intf)
{
    switch (intf) {
    case NET_IF_STA:
        return &if_wifi;
#ifdef SOFTAP_ENABLE
    case NET_IF_AP:
        return &if_wifi_ap;
#endif
    default:
        return NULL;
    }
}

int GetNetifIp(enum netifType intf, IpConfig *ip)
{
    struct netif *netif = GetNetif(intf);
    if (netif == NULL) {
        return -1;
    }
    if (ip_addr_isany(&netif->ip_addr)) {
        return -2;
    }
    if (ip) {
        memcpy(&ip->ipAddress, &netif->ip_addr, 4);
        memcpy(&ip->netmask, &netif->netmask, 4);
        memcpy(&ip->gateway, &netif->gw, 4);
        memcpy(&ip->dnsServers[0], dns_getserver(0), 4);
#if (WIFI_MAX_DNS_NUM > 1)
        memcpy(&ip->dnsServers[1], dns_getserver(1), 4);
#endif
    }
    return 0;
}
