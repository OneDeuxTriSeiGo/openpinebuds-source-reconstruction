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
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RTOS
# ifdef __WATCHER_DOG_RESET__
# define APP_WATCHDOG
# endif
#endif

#ifdef APP_WATCHDOG
int app_wdt_open(int seconds);

int app_wdt_reopen(int seconds);

int app_wdt_close(void);

int app_wdt_set_ping_time(int seconds);

/*
 * when enable, the watchdog ping timer will also check the ilde task's heath,
 * and if the idle task was not schedule for a period time,  there maybe
 * something wrong with system, and assert will happen.
 */
int app_wdt_hung_task_check_enable(int enable);

#else
static inline int app_wdt_open(int seconds) {return 0;}

static inline int app_wdt_reopen(int seconds) {return 0;}

static inline int app_wdt_close(void) {return 0;}

static inline int app_wdt_set_ping_time(int seconds) {return 0;}

static inline int app_wdt_hung_task_check_enable(int enable) {return 0;}

#endif


#ifdef __WATCHER_DOG_RESET__

#define  system_shutdown_wdt_config(seconds) \
                        do{ \
                            hal_wdt_stop(HAL_WDT_ID_0); \
                            hal_wdt_set_irq_callback(HAL_WDT_ID_0, NULL); \
                            hal_wdt_set_timeout(HAL_WDT_ID_0, seconds); \
                            hal_wdt_start(HAL_WDT_ID_0); \
                            hal_sleep_set_sleep_hook(HAL_SLEEP_HOOK_USER_OTA, NULL); \
                        }while(0)

int watchdog_hw_stop(void);

int watchdog_hw_start(int seconds);

void watchdog_ping(void);
#else

#define  system_shutdown_wdt_config(seconds)

static inline int watchdog_hw_stop(void) {return 0;}

static inline int watchdog_hw_start(int seconds) {return 0;}

static inline void watchdog_ping(void) {}

#endif

#ifdef __cplusplus
}
#endif

#endif  /*_ __WATCHDOG_H__*/
