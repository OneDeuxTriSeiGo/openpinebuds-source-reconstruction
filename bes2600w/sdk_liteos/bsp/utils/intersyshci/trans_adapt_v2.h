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
#ifndef __TRANS_ADAPT_V2_H__
#define __TRANS_ADAPT_V2_H__

#if defined(__cplusplus)
extern "C" {
#endif

void bes_pack_bt_hci_data(unsigned char type, unsigned short cmd_conn, unsigned short len, unsigned char *buffer);
int bes_pack_le_hci_data(unsigned char packet_type, unsigned char *packet, int size);
unsigned short hci_h4_receive_msg( const uint8_t *buf, uint32_t size);

#if defined(__cplusplus)
}
#endif

#endif /* __TRANS_ADAPT_V2_H__ */
