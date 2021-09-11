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
#ifndef __AT_THREAD_H__
#define __AT_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#define AT_MAILBOX_MAX (20)

typedef struct {
	uint32_t id;
	uint32_t ptr;
	uint32_t param0;
	uint32_t param1;
} AT_MESSAGE;

#define AT_MESSAGE_ID_CMD    0


typedef void (*AT_FUNC_T)(uint32_t, uint32_t);

typedef void (*AT_THREAD)(void*);
int at_os_init(void);
int at_enqueue_cmd(uint32_t cmd_id, uint32_t param,uint32_t pfunc);
#ifdef __cplusplus
	}
#endif

#endif // __AT_THREAD_H__

