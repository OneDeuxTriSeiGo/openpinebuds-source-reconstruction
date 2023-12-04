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
#ifndef __SPP_TASK_H__
#define __SPP_TASK_H__

#include "cmsis_os.h"
#include "cqueue.h"
#include "rfcomm_api.h"
#include "sdp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_HEADROOM    (100)

osThreadId create_spp_read_thread(void);

void close_spp_read_thread(void);

int spp_mailbox_put(struct spp_device *dev, uint8_t spp_dev_num, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SPP_TASK_H__ */
