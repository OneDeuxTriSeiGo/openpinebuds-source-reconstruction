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

#ifndef BESBT_CFG_H
#define BESBT_CFG_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct besbt_cfg_t{
    bool sniff;
    bool force_use_cvsd;
    bool avdtp_cp_enable;
    bool bt_source_enable;
    bool lhdc_v3;
};
extern struct besbt_cfg_t besbt_cfg;

#ifdef __cplusplus
}
#endif
#endif /* BESBT_H */
