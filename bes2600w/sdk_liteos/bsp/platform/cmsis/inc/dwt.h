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
#ifndef __DWT_H__
#define __DWT_H__

#ifdef __cplusplus
extern "C" {
#endif

#if __ARM_ARCH_PROFILE == 'M'
# define DWT_ENABLED
#endif

enum watchpoint_rw {
    WPT_READ  = 0,
    WPT_WRITE = 1,
    WPT_RW    = 2
};

#ifdef DWT_ENABLED
extern int dwt_enable(void);

extern int dwt_disable(void);

extern int watchpoint_enable(uint8_t wpt_index, uint32_t *word_address,
                                            enum watchpoint_rw access);

extern int watchpoint_disable(uint8_t wpt_index);

/*
 * return the number of supported watchpoints
 */
extern uint8_t watchpoint_get_numbers(void);

static inline int dwt_reset_cycle_cnt(void)
{
    DWT->CYCCNT = 0;
    /*start the cycle count*/
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    return 0;
}

static inline uint32_t dwt_read_cycle_cnt(void)
{
    uint32_t cnt = DWT->CYCCNT;

    /*stop the cycle count*/
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    return cnt;
}
#else

static inline int dwt_enable(void) {return -1;}

static inline int dwt_disable(void) {return -1;}

static inline int watchpoint_enable(uint8_t wpt_index, uint32_t *word_address,
                                            enum watchpoint_rw access)
{
   return -1;
}

static inline int watchpoint_disable(uint8_t wpt_index) {return -1;}

static inline int watchpoint_get_numbers(uint8_t wpt_index) {return 0;}

static inline int dwt_reset_cycle_cnt(void) {return 0;}

static inline uint32_t dwt_read_cycle_cnt(void) {return 0;}

#endif

#ifdef __cplusplus
}
#endif

#endif /* __DWT_H__ */
