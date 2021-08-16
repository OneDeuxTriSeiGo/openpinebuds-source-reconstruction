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
#ifndef __BES_WATCH_POINT__
#define __BES_WATCH_POINT__

#define BIT(x) (1 << (x))

/*armv8m*/
#define DWT_CTRL       0xE0001000  /*DWT Control Register*/
#define DWT_CYCCNT     0xE0001004  /*DWT Cycle Count Register*/
#define DWT_COMP0      0xE0001020  /*DWT Comparator Register n*/
#define DWT_FUNCTION0  0xE0001028  /*DWT Comparator Function Register n*/
#define DWT_VMASK0     0xE000102C  /*DWT Comparator Value Mask Register n*/
#define DCB_DEMCR      0xE000EDFC  /*Debug Exception and Monitor Control Register*/
#define DWT_DEVARCH    0xE0001FBC  /*DWT Device Architecture Register*/


/* DCB_DEMCR bit and field definitions */
#define TRCENA          BIT(24)
#define MON_EN          BIT(16)
#define VC_HARDERR      BIT(10)
#define VC_INTERR       BIT(9)
#define VC_BUSERR       BIT(8)
#define VC_STATERR      BIT(7)
#define VC_CHKERR       BIT(6)
#define VC_NOCPERR      BIT(5)
#define VC_MMERR        BIT(4)
#define VC_CORERESET    BIT(0)

#define DWT_OK               (0)
#define ERROR_FAIL           (-2)
#define DWT_LEN_ERR          (-307) 
#define DWT_ADDR_UNALIGNED   (-308)
#define DWT_PARA_ERR         (-309)

#ifdef __cplusplus
extern "C" {
#endif

enum watchpoint_rw {
    DWT_READ,     /*trigger dwt exception when read*/
    DWT_WRITE,    /*trigger dwt exception when write*/
    DWT_ACCESS,   /*trigger dwt exception when read or write*/
    DWT_MODE_OTHER
};

struct watchpoint {
    uint32_t address;
    uint32_t length;
    enum watchpoint_rw rw;
};

void ocd_watchpoint_setup(void);
int ocd_watchpoint_show(void);
int ocd_add_watch_point(uint32_t address, enum watchpoint_rw rw);
int ocd_remove_watch_point(uint32_t  address, enum watchpoint_rw rw);

#ifdef __cplusplus
}
#endif

#endif
