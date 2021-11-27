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
#include "plat_addr_map.h"
#include "hal_cmu.h"
#include "hal_location.h"
#include "hal_memsc.h"

#ifdef CHIP_SUBSYS_SENS
#include CHIP_SPECIFIC_HDR(reg_senscmu)

static struct SENSCMU_T * const cmu = (struct SENSCMU_T *)SENS_CMU_BASE;
#else
#include CHIP_SPECIFIC_HDR(reg_cmu)

static struct CMU_T * const cmu = (struct CMU_T *)CMU_BASE;
#endif

int hal_memsc_lock(enum HAL_MEMSC_ID_T id)
{
    if (id >= ARRAY_SIZE(cmu->MEMSC)) {
        return 0;
    }

    return cmu->MEMSC[id];
}

void hal_memsc_unlock(enum HAL_MEMSC_ID_T id)
{
    if (id >= ARRAY_SIZE(cmu->MEMSC)) {
        return;
    }

    cmu->MEMSC[id] = 1;
}

bool hal_memsc_avail(enum HAL_MEMSC_ID_T id)
{
    if (id >= ARRAY_SIZE(cmu->MEMSC)) {
        return false;
    }

    return !!(cmu->MEMSC_STATUS & (1 << id));
}

#ifdef AON_CMU_BASE
#include CHIP_SPECIFIC_HDR(reg_aoncmu)

static struct AONCMU_T * const aoncmu = (struct AONCMU_T *)AON_CMU_BASE;

int BOOT_TEXT_SRAM_LOC hal_memsc_aon_lock(enum HAL_MEMSC_AON_ID_T id)
{
    if (id >= ARRAY_SIZE(aoncmu->MEMSC)) {
        return 0;
    }

    return aoncmu->MEMSC[id];
}

void BOOT_TEXT_SRAM_LOC hal_memsc_aon_unlock(enum HAL_MEMSC_AON_ID_T id)
{
    if (id >= ARRAY_SIZE(aoncmu->MEMSC)) {
        return;
    }

    aoncmu->MEMSC[id] = 1;
}

bool hal_memsc_aon_avail(enum HAL_MEMSC_AON_ID_T id)
{
    if (id >= ARRAY_SIZE(aoncmu->MEMSC)) {
        return false;
    }

    return !!(aoncmu->MEMSC_STATUS & (1 << id));
}
#endif

