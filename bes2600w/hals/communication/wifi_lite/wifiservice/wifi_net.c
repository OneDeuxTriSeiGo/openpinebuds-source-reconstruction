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
#include "hal_trace.h"
#include <stdlib.h>
#include <string.h>
#include "station_info.h"
#include "bwifi_interface.h"
#include "wifi_hotspot_config.h"
#include "wifi_linked_info.h"
#include "cmsis_os.h"
// #include "bwifi_hal.h"
#include "lwip/tcpip.h"
#include "cmsis_os2.h"

extern int bwifi_get_own_mac(u8 *addr);
extern err_t ethernetif_init(struct netif *netif);
extern void ethernetif_input(u16_t devnum, void *p_buf, int size);

static void net_intf_status_change_cb(struct netif *netif)
{
    if (netif_is_up(netif) && !ip_addr_isany(&netif->ip_addr)) {
        uint32_t ip = ((ip4_addr_t *)&netif->ip_addr)->addr;
        uint32_t gw = ((ip4_addr_t *)&netif->gw)->addr;
        uint32_t mask = ((ip4_addr_t *)&netif->netmask)->addr;
        printf("net_intf_status_change_cb **ip = %s\n", inet_ntoa(ip));
        printf("net_intf_status_change_cb **netmask = %s\n", inet_ntoa(mask));
        printf("net_intf_status_change_cb **gw = %s\n", inet_ntoa(gw));
        WifiLinkedInfo *info = HalHmosGetWifiInfo();
        if (info) {
            info->ipAddress = ip;
            printf("net_intf_status_change_cb set info->ipAddress\n");
        }
    }
}



int HalTcpIpInit(void)
{
    int ret = 0;
    static uint8_t tcpip_init_flag = 0;
    if (tcpip_init_flag) {
        printf("%s %d is aready\n", __func__, __LINE__);
        return 0;
    } else {
        tcpip_init_flag = 1;
        printf("%s %d now\n", __func__, __LINE__);
        tcpip_init(NULL, NULL);
    }
    return ret;
}

int WifiDevNetifInit(struct netif *p_netif)
{
    u8 mac_addr[ETH_ALEN] = {0};
    static uint8_t NetIf_init_flag = 0;
    if (NetIf_init_flag == 1) {
        printf("%s %d is aready init\n", __func__, __LINE__);
        return 0;
    }
    NetIf_init_flag = 1;
    printf("%s %d start\n", __func__, __LINE__);
    bwifi_get_own_mac(mac_addr);
    lwip_netif_mac_addr_init(p_netif, mac_addr, ETH_ALEN);
    if (netif_add(p_netif, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4, NULL, ethernetif_init, tcpip_input) == 0) {
        printf("%s %d netif add wifi interface failed\n", __func__, __LINE__);
        return -1;
    }
    netif_set_default(p_netif);
    netif_set_status_callback(p_netif, net_intf_status_change_cb);
    printf("%s %d sucess\n", __func__, __LINE__);
    return 0;
}


int WifiHotspotNetifInit(struct netif *p_netif_ap)
{
    static uint8_t NetIf_init_flag = 0;
    if (NetIf_init_flag == 1) {
        printf("%s %d is aready init\n", __func__, __LINE__);
        return 0;
    }
    NetIf_init_flag = 1;
    u8 mac_addr[ETH_ALEN] = {0};
    if (p_netif_ap == NULL) {
        printf("%s %d p_netif_ap is null!!\n", __func__, __LINE__);
        return -1;
    }
    printf("%s %d start\n", __func__, __LINE__);
    bwifi_get_own_mac(mac_addr);
    mac_addr[ETH_ALEN] += 1;
    bwifi_reg_eth_input_handler(ethernetif_input);
    lwip_netif_mac_addr_init(p_netif_ap, mac_addr, ETH_ALEN);
    p_netif_ap->name[0] = 'a';
    p_netif_ap->name[1] = 'p';
    if (netif_add(p_netif_ap, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4, NULL, ethernetif_init, tcpip_input) == 0) {
        printf("%s %d netif add wifi interface failed\n", __func__, __LINE__);
        return -1;
    }
    printf("%s %d sucess\n", __func__, __LINE__);
    return 0;
}

void WifiHotspotNetifDeInit(struct netif *p_netif_ap)
{
    if (p_netif_ap != NULL) {
        netif_set_link_down(p_netif_ap);
        netif_set_down(p_netif_ap);
        netif_remove(p_netif_ap);
    }
    bwifi_unreg_eth_input_handler();   
}

