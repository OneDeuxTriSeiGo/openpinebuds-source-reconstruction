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
#ifndef __BT_DRV_IQ_COMMON_H__
#define  __BT_DRV_IQ_COMMON_H__
#if defined (RX_IQ_CAL) || defined(TX_PULLING_CAL)
#include "nvrecord_extension.h"

#define BTRF_REG_DUMP(addr) do{ \
                                uint16_t rf_reg_val = 0; \
                                btdrv_read_rf_reg(addr, &rf_reg_val); \
                                TRACE_IMM(2,"btrf reg addr:%04x val:%04x", addr, rf_reg_val); \
                            }while(0);

#define BTRF_DIG_DUMP(addr) do{ \
                                uint32_t dig_reg_val = 0; \
                                dig_reg_val = BTDIGITAL_REG(addr); \
                                TRACE_IMM(2,"dig reg addr:%08x val:%08x", addr, dig_reg_val); \
                            }while(0);

void bt_drv_rx_iq_datasort(int *arry,int len);

void bt_drv_rx_iq_datawrite(int * gain_arr, int * phy_arr, int len);

void lib_iqmis_getiq(short *data, int datalength_2X, float *din_i, float *din_q);

void lib_iqmis_esti(float *din_i, float *din_q, int din_length, int *gain_comp_reg, int *phi_comp_reg);

void btdrv_update_local_iq_rx_val(uint32_t freq_range, uint16_t gain_val, uint16_t phase_val, BT_IQ_RX_CALIBRATION_CONFIG_T* pConfig);

bool btdrv_get_iq_rx_val_from_nv(BT_IQ_RX_CALIBRATION_CONFIG_T* pConfig);

void btdrv_set_iq_rx_val_to_nv(BT_IQ_RX_CALIBRATION_CONFIG_T* pConfig);
#endif
#endif
