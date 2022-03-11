/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
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
#ifndef __USB_MSGQ_H__
#define __USB_MSGQ_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MSGQ_OK = 0,
    MSGQ_ERR_FULL,
    MSGQ_ERR_EMPTY,
    MSGQ_ERR_NOT_FOUND,
    MSGQ_ERR_NOT_INIT,
    MSGQ_ERR_OTHER_ERR
} MSGQ_ERROR;

typedef struct {
    void * msgs;        // array to store messages
    int pack_size;      // msg pack size
    int max_len;        // array length, that is to say how many msgs can be store
    int used_index;     // current filled array index
    int free_index;     // current free array index
    int current_index;  // current pointer location, maintained it just for msg_find
    int current_count;  // current msg count
} MSGQ;

void msgq_init(MSGQ * q, void * msgs, int pack_len, int msg_len);
void msgq_show(MSGQ *q);

void msgq_empty(MSGQ *q);
int msgq_add(MSGQ * q, void * msg);
int msgq_get(MSGQ * q, void * msg);

int msgq_register(MSGQ * q, void * msg);
void msgq_rewind(MSGQ *q);
void * msgq_find(MSGQ * q, int msg_id); // find the msg callback function according to the msg_id

#ifdef __cplusplus
}
#endif

#endif //__MSGQ_H__