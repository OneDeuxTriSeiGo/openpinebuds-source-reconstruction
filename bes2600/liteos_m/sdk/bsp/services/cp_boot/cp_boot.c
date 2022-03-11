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
#include "cmsis.h"
#include "hal_cmu.h"
#include "hal_dma.h"
#include "hal_location.h"
#include "hal_trace.h"
#include "hal_cache.h"
#include "string.h"
#include "cmsis_os.h"
#include "hal_timer.h"
#include "hal_mcu2cp.h"
#include "hal_cmu.h"
#include "hal_sysfreq.h"
#include "cp_ipc.h"
#ifdef WIFI_ON_CP
#include "bwifi_interface.h"
#include "wifi_srv.h"
#endif

extern uint32_t __cp_code_start[];
extern uint32_t __cp_code_end[];
extern uint32_t __cp_StackTop[];

#ifdef CP_DSP
extern void main2cp_init(void);
#endif

#ifdef CP_XIP
#define CP_BOOT_ENTRY  (CP_FLASHX_BASE)
#define CP_LOAD_ENTRY  (CP_FLASH_BASE
#else
#if (PSRAMCP_SIZE > 0)
#define CP_BOOT_ENTRY  (PSRAM_TO_PSRAMX(PSRAMCP_BASE))
#define CP_LOAD_ENTRY  (PSRAMCP_BASE)
#else
#define CP_BOOT_ENTRY  (0x00200000 + RAM_SIZE)
#define CP_LOAD_ENTRY  (0x20000000 + RAM_SIZE)
#endif
#endif

#ifdef DSP_IMAGE_COMP
extern int dsp_code_decom (unsigned char *outStream, unsigned int *uncompressedSize,
			      unsigned char *inStream,  unsigned int  length);
#endif
void cp_boot(void)
{
    TRACE(0, "%s", __FUNCTION__);

    hal_cmu_cp_disable();
    hal_sysfreq_req(HAL_SYSFREQ_USER_INIT, HAL_CMU_FREQ_390M);

#ifndef CP_XIP
    uint32_t remains;
    /* load cp image */
    remains = ((uint32_t)__cp_code_end - (uint32_t)__cp_code_start);//length of byte
#ifdef DSP_IMAGE_COMP
    /* compress cp bin */
    unsigned int uncompressed_size = 0;
    TRACE(0, "cp decompress start");
    int ret = dsp_code_decom ((unsigned char *)CP_LOAD_ENTRY, &uncompressed_size, (unsigned char *)__cp_code_start, remains);
    TRACE(0, "cp decompress end");
    if (ret != 0) {
        TRACE(2|TR_ATTR_IMM, "%s:%d FAIL!", __FUNCTION__, __LINE__);
        return;
    }
    hal_cache_sync_all(HAL_CACHE_ID_D_CACHE);
#else // !DSP_IMAGE_COMP
    TRACE(0, "load cp image start 0x%p", (void *)CP_LOAD_ENTRY);
    memcpy((unsigned char *)CP_LOAD_ENTRY, (unsigned char *)__cp_code_start, remains);
    TRACE(0, "load cp image end 0x%p", (void *)CP_BOOT_ENTRY);
#endif // DSP_IMAGE_COMP
#endif

#if defined(CONFIG_RPTUN)
#if !defined(CONFIG_BES_RPTUN_DELAY_BOOT)
    bes_rptun_heap_init();
#endif
#endif
    cp_ipc_init();
    hal_sysfreq_req(HAL_SYSFREQ_USER_INIT, HAL_CMU_FREQ_52M);
    hal_cmu_cp_boot(CP_BOOT_ENTRY);
    cp_ipc_start();
    osDelay(100);
#ifdef CP_DSP
    main2cp_init();
#endif
    hal_sysfreq_req(HAL_SYSFREQ_USER_INIT, HAL_CMU_FREQ_390M);
}

osThreadId cp_reboot_thread_id = NULL;
static void __cp_reboot_thread(void const* arg)
{
    TRACE(1, "\n\n%-32s start......\n", __FUNCTION__);
    cp_boot();
#ifdef  WIFI_ON_CP
    (void) bwifi_reinit();
#endif
    cp_reboot_thread_id = NULL;
    TRACE(1, "%s exit......", __FUNCTION__);

    osThreadExit();
}
osThreadDef(__cp_reboot_thread, osPriorityBelowNormal, 1, 4*1024, "cp_reboot_thread");

void cp_reboot(void)
{
    /* check if previous reboot thread is running */
    if (cp_reboot_thread_id)
        return;

    cp_reboot_thread_id = osThreadCreate(osThread(__cp_reboot_thread), NULL);
    if (cp_reboot_thread_id == NULL){
        printf("%s %d fail to create cp_reboot_thread_id task\n",__func__,__LINE__);
    }
    printf("%s create cp_reboot_thread_id task = 0x%X", __func__, (uint32_t)cp_reboot_thread_id);
}

void cp_stop(void)
{
    hal_cmu_cp_disable();
}
