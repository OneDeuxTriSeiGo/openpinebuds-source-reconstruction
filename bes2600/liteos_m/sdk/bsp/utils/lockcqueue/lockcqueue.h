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
#ifndef LOCKCQUEUE_H
#define LOCKCQUEUE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cqueue.h"

struct lockcqueue {
    CQueue cqueue;
    osMutexId queue_mutex_id;
    uint32_t os_mutex_cb_name[3];
    osMutexDef_t os_mutex_def_name;
};

int lockcqueue_init(struct lockcqueue *q, uint32_t size, uint8_t *buf);
int lockcqueue_enqueue(struct lockcqueue *q, uint8_t *buf, uint32_t size);
int lockcqueue_dequeue(struct lockcqueue *q, uint8_t *buf, uint32_t size);

#if defined(__cplusplus)
}
#endif

#endif /* LOCKCQUEUE_H */
