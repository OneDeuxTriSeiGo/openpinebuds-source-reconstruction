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
#include "hal_memsc.h"
#include "hal_cmu.h"

int hal_memsc_lock(enum HAL_MEMSC_ID_T id)
{
    if (id >= HAL_MEMSC_ID_QTY) {
        return 0;
    }

    return (hal_cmu_get_memsc_addr())[id];
}

void hal_memsc_unlock(enum HAL_MEMSC_ID_T id)
{
    if (id >= HAL_MEMSC_ID_QTY) {
        return;
    }

    (hal_cmu_get_memsc_addr())[id] = 1;
}

bool hal_memsc_avail(enum HAL_MEMSC_ID_T id)
{
    if (id >= HAL_MEMSC_ID_QTY) {
        return false;
    }

    return !!((hal_cmu_get_memsc_addr())[4] & (1 << id));
}
