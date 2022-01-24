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

#ifndef DHCPD_CONF_H_
#define DHCPD_CONF_H_

#define DHCPD_MAX_LEASES	        15
#define DEFAULT_START			"192.168.51.2"
#define DEFAULT_END			    "192.168.51.253"
#define DEFAULT_LEASE_TIME		        3600
#define DEFAULT_AUTO_TIME		        3
#define DEFAULT_CONFLICT_TIME	        3600
#define DEFAULT_DECLINE_TIME	        3600
#define DEFAULT_MIN_LEASE_TIME	        60
#define DEFAULT_SNAME			"BES"
#define DEFAULT_BOOT_FILE		""
#define DEFAULT_SERVER_IP       "192.168.0.1"
#define DEFAULT_BOOT_IP			"192.168.0.5"
#define DEFAULT_MASK			"255.255.255.0"
#define DEFAULT_GATEWAY			"192.168.0.1"
#define DEFAULT_DNS			    "168.95.1.1"
#define DEFAULT_DOMAIN			"www.bestechnic.com"
#define DHCPD_DEBUG

#define _string(s) #s
#define	STR(s)	_string(s)

#endif /* DHCPD_CONF_H_ */