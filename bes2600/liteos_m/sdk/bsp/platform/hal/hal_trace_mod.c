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
#include "hal_trace_mod.h"
#include "plat_types.h"

#undef _TR_MODULE_DEF_A
#define _TR_MODULE_DEF_A(p, m)              # m

static const char * mod_desc[] = {
    _TR_MODULE_LIST
};

const char *hal_trace_get_log_module_desc(enum TR_MODULE_T module)
{
    if (module < ARRAY_SIZE(mod_desc)) {
        return mod_desc[module];
    }
    return NULL;
}

