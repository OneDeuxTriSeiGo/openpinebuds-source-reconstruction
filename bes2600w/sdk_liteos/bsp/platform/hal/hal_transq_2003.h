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
#ifdef CHIP_HAS_TRANSQ

#include "plat_addr_map.h"
#include "reg_transq.h"
#include "hal_transq.h"
#include "hal_trace.h"
#include "hal_cmu.h"
#include "hal_sleep.h"
#include "hal_cache.h"
#include "hal_chipid.h"
//#include "string.h"
#include "stdbool.h"
#include "cmsis_nvic.h"
#define READ_REG(b,a) \
     (*(volatile uint32_t *)(b+a))

#define WRITE_REG(v,b,a) \
     ((*(volatile uint32_t *)(b+a)) = v)
     
#define REG(a)          *(volatile uint32_t *)(a)

// BITMAP:
// [High Priority Slots] ...... [Normal Priority Slots]
// 31 30 29 28 27 26 25  ...... 10 9 8 7 6 5 4 3 2 1 0

static struct TRANSQ_T * const transq[HAL_TRANSQ_ID_QTY] = {
    (struct TRANSQ_T *)TRANSQ0_BASE,
#if (CHIP_HAS_TRANSQ > 1)
    (struct TRANSQ_T *)TRANSQ1_BASE,
#endif
};

static struct TRANSQ_T * const peer_transq[HAL_TRANSQ_ID_QTY] = {
    (struct TRANSQ_T *)TRANSQ0_PEER_BASE,
#if (CHIP_HAS_TRANSQ > 1)
    (struct TRANSQ_T *)TRANSQ1_PEER_BASE,
#endif
};

static const enum HAL_CMU_MOD_ID_T transq_mod[HAL_TRANSQ_ID_QTY] = {
    HAL_CMU_MOD_P_TRANSQ0,
#if (CHIP_HAS_TRANSQ > 1)
    HAL_CMU_MOD_P_TRANSQ1,
#endif
};

static const IRQn_Type remote_irq_num[HAL_TRANSQ_ID_QTY] = {
    TRANSQ0_RMT_IRQn,
#if (CHIP_HAS_TRANSQ > 1)
    TRANSQ1_RMT_IRQn,
#endif
};

static const IRQn_Type local_irq_num[HAL_TRANSQ_ID_QTY] = {
    TRANSQ0_LCL_IRQn,
#if (CHIP_HAS_TRANSQ > 1)
    TRANSQ1_LCL_IRQn,
#endif
};
uint8_t transq_slot_num[HAL_TRANSQ_ID_QTY] =
{
    
    TRANSQ_SLOT_NUM_WIFI,
#if (CHIP_HAS_TRANSQ > 1)
    TRANSQ_SLOT_NUM_DSP,
#endif

};

static struct HAL_TRANSQ_CFG_T transq_cfg[HAL_TRANSQ_ID_QTY];

static uint8_t next_tx_slot[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];

static uint8_t active_tx_slot[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];

static bool tx_slot_full[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];

static uint8_t next_rx_slot[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];

static uint32_t rx_irq_mask[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];
static uint32_t rx_irq_mask_e[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];
static uint32_t tx_irq_mask[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];
static uint32_t tx_irq_mask_e[HAL_TRANSQ_ID_QTY][HAL_TRANSQ_PRI_QTY];


extern volatile uint32_t* wifi_tranq_next;
extern volatile uint32_t* wifi_tranq_active;
extern volatile uint32_t* wifi_error;
extern volatile uint32_t* wifi_error1;
static uint32_t TxOutstanding = 0;



static uint32_t construct_mask(uint32_t lsb, uint32_t width)
{
    int i;
    uint32_t result;

    if (lsb >= 32 || width == 0) {
        return 0;
    }

    result = 0;
    for (i = lsb; i < lsb + width; i++) {
        result |= (1 << i);
    }

    return result;
}

static uint32_t get_next_rx_slot(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, uint32_t slot)
{
    slot++;
    if (pri == HAL_TRANSQ_PRI_HIGH) {
        if (slot >= transq_slot_num[id]) {
            slot =  transq_slot_num[id] - transq_cfg[id].slot.rx_num[pri];
        }
    } else {
        if (slot >= transq_cfg[id].slot.rx_num[pri]) {
            slot = 0;
        }
    }

    return slot;
}

static uint32_t get_next_tx_slot(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, uint32_t slot)
{
    slot++;
    if (pri == HAL_TRANSQ_PRI_HIGH) {
        if (slot >=  transq_slot_num[id]) {
            slot =  transq_slot_num[id] - transq_cfg[id].slot.tx_num[pri];
        }
    } else {
        if (slot >= transq_cfg[id].slot.tx_num[pri]) {
            slot = 0;
        }
    }
	//TRACE(1,"get_next_tx_slot,pri=%d, tx_num= %d,slot=%d" ,pri,transq_cfg[id].slot.tx_num[pri],slot);

    return slot;
}
#ifdef __ARM_ARCH_ISA_ARM
#ifdef RTOS
static void hal_transq_remote_irq_handler(int irq_num, void * irq_data)
#else
static void hal_transq_remote_irq_handler(void)
#endif
#else
static void hal_transq_remote_irq_handler(void)
#endif
{
    enum HAL_TRANSQ_ID_T id;
    enum HAL_TRANSQ_PRI_T pri = HAL_TRANSQ_PRI_NORMAL;
    uint32_t status,status1;
    uint32_t slot;
    //uint32_t slot_tmp = 0;

#if (CHIP_HAS_TRANSQ > 1)
#ifdef __ARM_ARCH_ISA_ARM
#ifdef RTOS
#ifdef KERNEL_LITEOS_A
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#else
    IRQn_Type irq = (IRQn_Type)irq_num;
#endif
#else
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#endif
#else
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#endif

    for (id = HAL_TRANSQ_ID_0; id < HAL_TRANSQ_ID_QTY; id++) {
        if (irq == remote_irq_num[id]) {
            break;
        }
    }
    if (id >= HAL_TRANSQ_ID_QTY) {
        return;
    }
#else
    id = HAL_TRANSQ_ID_0;
#endif

    while (((status = peer_transq[id]->RMT_MIS) != 0)||((status1 = peer_transq[id]->RMT_MIS_E) != 0)) {
        status1 = peer_transq[id]->RMT_MIS_E;
        if ((status & rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH]) ||(status1 & rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH])) {
            pri = HAL_TRANSQ_PRI_HIGH;
        } else if ((status & rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL]) ||(status1 & rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL])){
            pri = HAL_TRANSQ_PRI_NORMAL;
        } else {
            ASSERT(false, "TRANSQ-%d: Corrupted rx mask: status=0x%08X rx_mask=0x%08X / 0x%08X",
                id, status, rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH], rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL]);
        }
        slot = next_rx_slot[id][pri];
        ASSERT(slot <  transq_slot_num[id] && (status & (1 << slot)),
            "TRANSQ-: Rx IRQ  status=0x%08X 0x%08X rx_mask=0x%08X 0x%08x slot=%d,irq_count:%d",
            status,status1 , rx_irq_mask[id][pri], rx_irq_mask_e[id][pri],slot,transq_cfg[id].rx_irq_count);

        // Mask IRQ from corresponding slots
        peer_transq[id]->RMT_INTMASK &= ~rx_irq_mask[id][pri];
		peer_transq[id]->RMT_INTMASK_E &= ~rx_irq_mask_e[id][pri];
        // Force to flush the write FIFO in the bus path
        peer_transq[id]->RMT_INTMASK;

        transq_cfg[id].rx_irq_count ++;
        if (transq_cfg[id].rx_handler) {
            transq_cfg[id].rx_handler(pri);
        }
    }
}

static int hal_transq_active_tx_valid(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri)
{
    return ((active_tx_slot[id][pri] >=  transq_slot_num[id]) ||
            (active_tx_slot[id][pri] != next_tx_slot[id][pri]) ||
            tx_slot_full[id][pri]);
}

#ifdef __ARM_ARCH_ISA_ARM
#ifdef RTOS
void hal_transq_local_irq_handler(int irq_num, void *irq_data)
#else
static void hal_transq_local_irq_handler(void)
#endif
#else
static void hal_transq_local_irq_handler(void)
#endif
{
    enum HAL_TRANSQ_ID_T id;
    enum HAL_TRANSQ_PRI_T pri = HAL_TRANSQ_PRI_NORMAL;
    uint32_t status,status1;
    uint32_t slot, next_slot;
    uint32_t lock;

#if (CHIP_HAS_TRANSQ > 1)
#ifdef __ARM_ARCH_ISA_ARM
#ifdef RTOS
#ifdef KERNEL_LITEOS_A
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#else
    IRQn_Type irq = (IRQn_Type)irq_num;
#endif
#else
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#endif
#else
    IRQn_Type irq = NVIC_GetCurrentActiveIRQ();
#endif

    for (id = HAL_TRANSQ_ID_0; id < HAL_TRANSQ_ID_QTY; id++) {
        if (irq == local_irq_num[id]) {
            break;
        }
    }
    if (id >= HAL_TRANSQ_ID_QTY) {
        return;
    }
#else
    id = HAL_TRANSQ_ID_0;
#endif

    while (((status = transq[id]->LERR_MIS) != 0)||((status1 = transq[id]->LERR_MIS_E) != 0)) {
	    status1 = transq[id]->LERR_MIS_E;
        if(status)
            transq[id]->LERR_ISC.LERR_INTCLR = status;
        if(status1)
            transq[id]->LERR_ISC_E.LERR_INTCLR = status1;
        ASSERT(false, "TRANSQ-%d: Tx on active slot: 0x%08x", id, status);
    }

    while (((status = transq[id]->LDONE_MIS) != 0)||((status1 = transq[id]->LDONE_MIS_E) != 0)) {
        status1= transq[id]->LDONE_MIS_E;
        
        //TRACE(1,"transq_cfg[id].tx_handler= 0x%08x mask: 0x%08x, 0x%08x" ,transq_cfg[id].tx_handler,transq[id]->LDONE_INTMASK,transq[id]->LDONE_INTMASK_E );
        if (transq_cfg[id].tx_handler) {
            lock = int_lock();

            ASSERT(hal_transq_active_tx_valid(id, HAL_TRANSQ_PRI_HIGH), "TRANSQ-%d: Corrupted pri active tx: active=%d next=%d full=%d",
                id, active_tx_slot[id][HAL_TRANSQ_PRI_HIGH], next_tx_slot[id][HAL_TRANSQ_PRI_HIGH], tx_slot_full[id][HAL_TRANSQ_PRI_HIGH]);
            ASSERT(hal_transq_active_tx_valid(id, HAL_TRANSQ_PRI_NORMAL), "TRANSQ-%d: Corrupted active tx: active=%d next=%d full=%d",
                id, active_tx_slot[id][HAL_TRANSQ_PRI_NORMAL], next_tx_slot[id][HAL_TRANSQ_PRI_NORMAL], tx_slot_full[id][HAL_TRANSQ_PRI_NORMAL]);

            if ((status & tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH]) ||(status1 & tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH])){
                pri = HAL_TRANSQ_PRI_HIGH;
            } else if ((status & tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL])  ||(status1 & tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL])){
                pri = HAL_TRANSQ_PRI_NORMAL;
            } else {
                ASSERT(false, "TRANSQ-%d: Corrupted tx mask: status=0x%08X tx_mask=0x%08X / 0x%08X",
                    id, status, tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL], tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL]);
            }
            slot = active_tx_slot[id][pri];
            //TRACE(1,"local irq, active_tx_slot= %d" ,slot);
            //*wifi_tranq_active = slot;
            //* wifi_error = status;
            //* wifi_error1 = status1;
            ASSERT(slot <  transq_slot_num[id] && ((status & (1 << slot))||(status1 & (1 << (slot-32)))),
                "TRANSQ-%d: Tx done IRQ when slot empty or out of order: status=0x%08x status1=0x%08x next=%d pri_next=%d",
                id, status,status1, active_tx_slot[id][HAL_TRANSQ_PRI_NORMAL], active_tx_slot[id][HAL_TRANSQ_PRI_HIGH]);

            // Clear the interrupt
            if(slot < 32)
                transq[id]->LDONE_ISC.LDONE_INTCLR = (1 << slot);
            else
                transq[id]->LDONE_ISC_E.LDONE_INTCLR = (1 << (slot-32));

            next_slot = get_next_tx_slot(id, pri, slot);

            if (!tx_slot_full[id][pri] && (next_slot == next_tx_slot[id][pri])) {
                // No tx in progress
                active_tx_slot[id][pri] =  transq_slot_num[id];
            } else {
                tx_slot_full[id][pri] = false;
                if (transq_cfg[id].slot.tx_num[pri] == 1) {
                    // No tx in progress
                    active_tx_slot[id][pri] =  transq_slot_num[id];
                } else {
                    // Some tx in progress
                    active_tx_slot[id][pri] = next_slot;
                }
            }
            //TRACE(1,"local irq, active_tx_slot[id][pri]= %d" ,active_tx_slot[id][pri]);
           if (TxOutstanding) {
                TxOutstanding--;
                if (!TxOutstanding) {
                    hal_sys_wake_unlock(HAL_SYS_WAKE_LOCK_USER_TRANSQ);
                }
            }

            int_unlock(lock);

            transq_cfg[id].tx_handler(pri,
                (const uint8_t *)transq[id]->WSLOT[slot].ADDR,
                transq[id]->WSLOT[slot].LEN);
        } else {
            transq[id]->LDONE_INTMASK = 0;
            transq[id]->LDONE_INTMASK_E = 0;
        }
    }
}

enum HAL_TRANSQ_RET_T hal_transq_get_rx_status(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, bool *ready)
{
    uint32_t lock;
    uint32_t slot;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (pri >= HAL_TRANSQ_PRI_QTY) {
        return HAL_TRANSQ_RET_BAD_PRI;
    }
    if (transq_cfg[id].slot.rx_num[pri] == 0) {
        return HAL_TRANSQ_RET_BAD_RX_NUM;
    }
    if (transq_cfg[id].rx_handler) {
        // Rx will be processed by IRQ handler
        return HAL_TRANSQ_RET_BAD_MODE;
    }

    lock = int_lock();

    slot = next_rx_slot[id][pri];

    if ((slot <  transq_slot_num[id]) && (peer_transq[id]->RMT_ISC.RMT_RIS & (1 << slot))) {
        *ready = true;
    } else {
        *ready = false;
    }

    int_unlock(lock);

    return HAL_TRANSQ_RET_OK;
}

enum HAL_TRANSQ_RET_T hal_transq_get_tx_status(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, bool *done)
{
    uint32_t lock;
    uint32_t slot, next_slot;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (pri >= HAL_TRANSQ_PRI_QTY) {
        return HAL_TRANSQ_RET_BAD_PRI;
    }
    if (transq_cfg[id].slot.tx_num[pri] == 0) {
        return HAL_TRANSQ_RET_BAD_TX_NUM;
    }
    if (transq_cfg[id].tx_handler) {
        // Tx done will be processed by IRQ handler
        return HAL_TRANSQ_RET_BAD_MODE;
    }

    lock = int_lock();

    slot = active_tx_slot[id][pri];

    ASSERT(hal_transq_active_tx_valid(id, pri), "TRANSQ-%d: Corrupted active tx: pri=%d active=%d next=%d full=%d",
        id, pri, active_tx_slot[id][pri], next_tx_slot[id][pri], tx_slot_full[id][pri]);

    if (transq[id]->LDONE_ISC.LDONE_RIS & (1 << slot)) {
        *done = true;

        // Clear the interrupt
        transq[id]->LDONE_ISC.LDONE_INTCLR = (1 << slot);

        next_slot = get_next_tx_slot(id, pri, slot);

        if (!tx_slot_full[id][pri] && (next_slot == next_tx_slot[id][pri])) {
            // No tx in progress
            active_tx_slot[id][pri] =  transq_slot_num[id];
        } else {
            tx_slot_full[id][pri] = false;
            if (transq_cfg[id].slot.tx_num[pri] == 1) {
                // No tx in progress
                active_tx_slot[id][pri] =  transq_slot_num[id];
            } else {
                // Some tx in progress
                active_tx_slot[id][pri] = next_slot;
            }
        }
    } else {
        *done = false;
    }

    int_unlock(lock);

    return HAL_TRANSQ_RET_OK;
}

bool hal_transq_tx_busy(enum HAL_TRANSQ_ID_T id)
{
    bool ret;
    uint32_t lock;
    lock = int_lock();

    if ((active_tx_slot[id][HAL_TRANSQ_PRI_NORMAL] !=  transq_slot_num[id] ||
                active_tx_slot[id][HAL_TRANSQ_PRI_HIGH] !=  transq_slot_num[id])
            && (transq[id]->LDONE_MIS == 0)) {
        ret = true;
    } else {
        ret = false;
    }

    int_unlock(lock);
    return ret;
}
enum HAL_TRANSQ_RET_T hal_transq_rx_first(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t **data, uint32_t *len)
{
    enum HAL_TRANSQ_RET_T ret;
    uint32_t slot;
    //uint32_t lock;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (pri >= HAL_TRANSQ_PRI_QTY) {
        return HAL_TRANSQ_RET_BAD_PRI;
    }
    if (transq_cfg[id].slot.rx_num[pri] == 0) {
        return HAL_TRANSQ_RET_BAD_RX_NUM;
    }

    //lock = int_lock();

    slot = next_rx_slot[id][pri];
    
    //TRACE(1,"hal_transq_rx_first, slot= %d" ,slot);
    if(slot <32)
    {

    if ((slot <  transq_slot_num[id]) && (peer_transq[id]->RMT_ISC.RMT_RIS & (1 << slot))) {
        // Msg available
        ret = HAL_TRANSQ_RET_OK;

        if (data) {
            *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
        }
        if (len) {
            *len = peer_transq[id]->RSLOT[slot].LEN;
        }
    } else {
        // No msg. Re-enable IRQ
        ret = HAL_TRANSQ_RET_RX_EMPTY;
       // wlan_func.transq_irq_count =0;
        transq_cfg[id].rx_irq_count =0;
        if (data) {
            *data = NULL;
        }
        if (len) {
            *len = 0;
        }
        peer_transq[id]->RMT_INTMASK |= rx_irq_mask[id][pri];
            peer_transq[id]->RMT_INTMASK_E |= rx_irq_mask_e[id][pri];
        }
    }else{
        if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC_E.RMT_RIS & (1 << (slot-32))) {
            // Msg available
            ret = HAL_TRANSQ_RET_OK;

            if (data) {
                *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
            }
            if (len) {
                *len = peer_transq[id]->RSLOT[slot].LEN;
            }
        } else {
            // No msg. Re-enable IRQ
            ret = HAL_TRANSQ_RET_RX_EMPTY;
       // wlan_func.transq_irq_count =0;
        transq_cfg[id].rx_irq_count =0;

            if (data) {
                *data = NULL;
            }
            if (len) {
                *len = 0;
            }
            peer_transq[id]->RMT_INTMASK_E |= rx_irq_mask[id][pri];
            peer_transq[id]->RMT_INTMASK_E |= rx_irq_mask_e[id][pri];
        }

    }

    //int_unlock(lock);

    return ret;
}

enum HAL_TRANSQ_RET_T hal_transq_rx_next(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t **data, uint32_t *len)
{
    enum HAL_TRANSQ_RET_T ret;
    uint32_t slot;
    //uint32_t lock;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (pri >= HAL_TRANSQ_PRI_QTY) {
        return HAL_TRANSQ_RET_BAD_PRI;
    }
    if (transq_cfg[id].slot.rx_num[pri] == 0) {
        return HAL_TRANSQ_RET_BAD_RX_NUM;
    }

    ret = HAL_TRANSQ_RET_OK;

    //lock = int_lock();

    slot = next_rx_slot[id][pri];
    
    //TRACE(1,"hal_transq_rx_next, slot= %d" ,slot);
    if(slot <32)
    {
        if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC.RMT_RIS & (1 << slot)) {
            // Clear cur IRQ
            peer_transq[id]->RMT_ISC.RMT_INTCLR = (1 << slot);

        // Update next_rx_slot
        slot = get_next_rx_slot(id, pri, slot);
        
        next_rx_slot[id][pri] = slot;
            if(slot <32)
            {
        if ((slot <  transq_slot_num[id]) && (peer_transq[id]->RMT_ISC.RMT_RIS & (1 << slot))) {
            // Next msg available
            if (data) {
                *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
            }
            if (len) {
                *len = peer_transq[id]->RSLOT[slot].LEN;
                    }
                } else {
                    // No msg
                    ret = HAL_TRANSQ_RET_RX_EMPTY;
                }
            }else{
                if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC_E.RMT_RIS & (1 << (slot-32))) {
                    // Next msg available
                    if (data) {
                        *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
                    }
                    if (len) {
                        *len = peer_transq[id]->RSLOT[slot].LEN;
                    }
                } else {
                    // No msg
                    ret = HAL_TRANSQ_RET_RX_EMPTY;
                }
            }
        } else {
            // No msg
            ret = HAL_TRANSQ_RET_RX_EMPTY;
        }
    }else{
        if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC_E.RMT_RIS & (1 << (slot-32))) {
            // Clear cur IRQ
            peer_transq[id]->RMT_ISC_E.RMT_INTCLR = (1 << (slot-32));

            // Update next_rx_slot
            slot = get_next_rx_slot(id, pri, slot);
            next_rx_slot[id][pri] = slot;
            if(slot <32)
            {
                if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC.RMT_RIS & (1 << (slot))) {
                    // Next msg available
                    if (data) {
                        *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
                    }
                    if (len) {
                        *len = peer_transq[id]->RSLOT[slot].LEN;
                    }
                } else {
                    // No msg
                    ret = HAL_TRANSQ_RET_RX_EMPTY;
                }
            }else{
                if (slot <  transq_slot_num[id] && peer_transq[id]->RMT_ISC_E.RMT_RIS & (1 << (slot-32))) {
                    // Next msg available
                    if (data) {
                        *data = (const uint8_t *)peer_transq[id]->RSLOT[slot].ADDR;
                    }
                    if (len) {
                        *len = peer_transq[id]->RSLOT[slot].LEN;
                    }
                } else {
                    // No msg
                    ret = HAL_TRANSQ_RET_RX_EMPTY;
                }
            }
        } else {
            // No msg
            ret = HAL_TRANSQ_RET_RX_EMPTY;
        }

    }
    if (ret == HAL_TRANSQ_RET_RX_EMPTY) {
        if (data) {
            *data = NULL;
        }
        if (len) {
            *len = 0;
        }
        //wlan_func.transq_irq_count =0;
        transq_cfg[id].rx_irq_count =0;
        // Re-enable IRQ
        peer_transq[id]->RMT_INTMASK |= rx_irq_mask[id][pri];
        peer_transq[id]->RMT_INTMASK_E |= rx_irq_mask_e[id][pri];
    }

    //int_unlock(lock);

    return ret;
}

enum HAL_TRANSQ_RET_T hal_transq_tx(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t *data, uint32_t len)
{
    enum HAL_TRANSQ_RET_T ret;
    uint32_t lock;
    uint32_t slot;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (pri >= HAL_TRANSQ_PRI_QTY) {
        return HAL_TRANSQ_RET_BAD_PRI;
    }
    if (transq_cfg[id].slot.tx_num[pri] == 0) {
        return HAL_TRANSQ_RET_BAD_TX_NUM;
    }

    lock = int_lock();
    if (tx_slot_full[id][pri]) {
        ret = HAL_TRANSQ_RET_TX_FULL;
    } else {
        TxOutstanding++;
        if (TxOutstanding == 1) {
            hal_sys_wake_lock(HAL_SYS_WAKE_LOCK_USER_TRANSQ);
        }

        ret = HAL_TRANSQ_RET_OK;

        slot = next_tx_slot[id][pri];
        //TRACE(1,"hal_transq_tx, slot= %d, %d, %d" ,slot, next_tx_slot[id][pri],pri);

        transq[id]->WSLOT[slot].ADDR = (uint32_t)data;
        transq[id]->WSLOT[slot].LEN = len;
        if(slot < 32)
        {
            transq[id]->RMT_INTSET = (1 << slot);
        }else{
            transq[id]->RMT_INTSET_E = (1 << (slot-32));

        }

        // Update active_tx_slot if this is the only tx in progress
        if (active_tx_slot[id][pri] >=  transq_slot_num[id]) {
            active_tx_slot[id][pri] = slot;
        }

        // Update next_tx_slot
        next_tx_slot[id][pri] = get_next_tx_slot(id, pri, slot);
        //*wifi_tranq_next = next_tx_slot[id][pri];
        //*wifi_tranq_active = active_tx_slot[id][pri];

        if (next_tx_slot[id][pri] == active_tx_slot[id][pri]) {
            tx_slot_full[id][pri] = true;
        }
    }

    int_unlock(lock);

    return ret;
}

enum HAL_TRANSQ_RET_T hal_transq_update_num(enum HAL_TRANSQ_ID_T id, const struct HAL_TRANSQ_SLOT_NUM_T *slot)
{
   // TRACE(1,"transq_lot_num[id]=%d, rx_num :%d, %d, tx_num: %d,%d " ,transq_slot_num[id] ,
	//	                slot->rx_num[HAL_TRANSQ_PRI_NORMAL],slot->rx_num[HAL_TRANSQ_PRI_HIGH],
	//	                slot->tx_num[HAL_TRANSQ_PRI_NORMAL],slot->tx_num[HAL_TRANSQ_PRI_HIGH]);

    uint32_t rx_mask;
    uint32_t rx_mask_e;
    uint32_t tx_mask;
    uint32_t tx_mask_e;
    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (slot == NULL) {
        return HAL_TRANSQ_RET_BAD_SLOT;
    }
    if (slot->tx_num[HAL_TRANSQ_PRI_NORMAL] + slot->tx_num[HAL_TRANSQ_PRI_HIGH] >  transq_slot_num[id]) {
        return HAL_TRANSQ_RET_BAD_TX_NUM;
    }
    if (slot->rx_num[HAL_TRANSQ_PRI_NORMAL] + slot->rx_num[HAL_TRANSQ_PRI_HIGH] >  transq_slot_num[id]) {
        return HAL_TRANSQ_RET_BAD_RX_NUM;
    }

    transq_cfg[id].slot = *slot;
    if( transq_slot_num[id] <=32)
    {
        rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = construct_mask(0, slot->rx_num[HAL_TRANSQ_PRI_NORMAL]);
        rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] = 0x00;
        rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(
             transq_slot_num[id] - slot->rx_num[HAL_TRANSQ_PRI_HIGH],
            slot->rx_num[HAL_TRANSQ_PRI_HIGH]);
        rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] =0x00;

		
		tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]);
		tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] = 0x00;
		
		tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] =  construct_mask(
			transq_slot_num[id]  - slot->tx_num[HAL_TRANSQ_PRI_HIGH],
			slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
		tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = 0;
#if 0
        //tx_mask = construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]) |
         //   construct_mask(TRANSQ_SLOT_NUM - slot->tx_num[HAL_TRANSQ_PRI_HIGH], slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
        tx_mask = rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH];
        tx_mask_e = rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH];
        transq[id]->RMT_INTMASK = tx_mask;
        transq[id]->LERR_INTMASK = tx_mask;
        
        transq[id]->RMT_INTMASK_E = tx_mask_e;
        transq[id]->LERR_INTMASK_E = tx_mask_e;
        if (transq_cfg[id].tx_handler) {
            transq[id]->LDONE_INTMASK = tx_mask;
            transq[id]->LDONE_INTMASK_E = tx_mask_e;
        } else {
            transq[id]->LDONE_INTMASK = 0;
        }
#endif
    }else{
        if(slot->rx_num[HAL_TRANSQ_PRI_NORMAL] <=32) 
        {
            rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = construct_mask(0, slot->rx_num[HAL_TRANSQ_PRI_NORMAL]);
            rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] = 0x00;
			
            if(slot->rx_num[HAL_TRANSQ_PRI_HIGH] <=(transq_slot_num[id] -32))
			{
	            rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = 0;
	            rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(
	                (transq_slot_num[id] -32) - slot->rx_num[HAL_TRANSQ_PRI_HIGH],
	                slot->rx_num[HAL_TRANSQ_PRI_HIGH]);
			}else{
			    
	            rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(transq_slot_num[id] - slot->rx_num[HAL_TRANSQ_PRI_HIGH],
	                slot->rx_num[HAL_TRANSQ_PRI_HIGH] -(transq_slot_num[id] -32) );
	            rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(0,
	                (transq_slot_num[id] -32) );
			}
        }else{
            rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = 0xFFFFFFFF;
            rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] =  construct_mask(0, slot->rx_num[HAL_TRANSQ_PRI_NORMAL]-32);
            
            rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = 0;
            rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(
                transq_slot_num[id]-32 - slot->rx_num[HAL_TRANSQ_PRI_HIGH],
                slot->rx_num[HAL_TRANSQ_PRI_HIGH]);
        }
        #if 0
        //tx_mask = construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]) |
         //   construct_mask(TRANSQ_SLOT_NUM - slot->tx_num[HAL_TRANSQ_PRI_HIGH], slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
        tx_mask = rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH];
        tx_mask_e = rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH];
        transq[id]->RMT_INTMASK = tx_mask;
        transq[id]->LERR_INTMASK = tx_mask;
        
        transq[id]->RMT_INTMASK_E = tx_mask_e;
        transq[id]->LERR_INTMASK_E = tx_mask_e;
        if (transq_cfg[id].tx_handler) {
            transq[id]->LDONE_INTMASK = tx_mask;
            transq[id]->LDONE_INTMASK_E = tx_mask_e;
        } else {
            transq[id]->LDONE_INTMASK = 0;
        }
        #endif
        if(slot->tx_num[HAL_TRANSQ_PRI_NORMAL] <=32) 
        {
            tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]);
            tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] = 0x00;

            if(slot->tx_num[HAL_TRANSQ_PRI_HIGH] <=(transq_slot_num[id] -32))
			{
	            tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = 0;
	            tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(
	                (transq_slot_num[id] -32) - slot->tx_num[HAL_TRANSQ_PRI_HIGH],
	                slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
			}else{
			    
	            tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(transq_slot_num[id] - slot->tx_num[HAL_TRANSQ_PRI_HIGH],
	                slot->tx_num[HAL_TRANSQ_PRI_HIGH] -(transq_slot_num[id] -32) );
	            tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(0,
	                (transq_slot_num[id] -32) );
			}
        }else{
            tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = 0xFFFFFFFF;
            tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] =  construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]-32);
            
            tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = 0;
            tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] = construct_mask(
                transq_slot_num[id]-32 - slot->tx_num[HAL_TRANSQ_PRI_HIGH],
                slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
        }

    }
    //tx_mask = construct_mask(0, slot->tx_num[HAL_TRANSQ_PRI_NORMAL]) |
     //   construct_mask(TRANSQ_SLOT_NUM - slot->tx_num[HAL_TRANSQ_PRI_HIGH], slot->tx_num[HAL_TRANSQ_PRI_HIGH]);
    rx_mask = rx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask[id][HAL_TRANSQ_PRI_HIGH];
    rx_mask_e = rx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] |rx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH];
    
    tx_mask = tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] |tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH];
    tx_mask_e = tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] |tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH];
    transq[id]->RMT_INTMASK = rx_mask;
    transq[id]->LERR_INTMASK = tx_mask;
    
    transq[id]->RMT_INTMASK_E = rx_mask_e;
    transq[id]->LERR_INTMASK_E = tx_mask_e;
	
	
    //TRACE(1,"transq_cfg[id].tx_handler 0x%08x,  0x%08x, 0x%08x, 0x%08x, 0x%08x" , 
	//	transq_cfg[id].tx_handler,rx_mask,rx_mask_e,tx_mask,tx_mask_e
	//);
    if (transq_cfg[id].tx_handler) {
        transq[id]->LDONE_INTMASK = tx_mask;
        transq[id]->LDONE_INTMASK_E = tx_mask_e;
    } else {
        transq[id]->LDONE_INTMASK = 0;
        transq[id]->LDONE_INTMASK_E = 0;
    }
    //tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL] = tx_irq_mask[id][HAL_TRANSQ_PRI_NORMAL];
    //tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] =  tx_irq_mask_e[id][HAL_TRANSQ_PRI_NORMAL] ;
    
    //tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH] = tx_irq_mask[id][HAL_TRANSQ_PRI_HIGH];
    //tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH] =tx_irq_mask_e[id][HAL_TRANSQ_PRI_HIGH];
    TRACE(1,"LDONE_INTMASK= 0x%x, 0x%x" ,transq[id]->LDONE_INTMASK ,transq[id]->LDONE_INTMASK_E);
    TRACE(1,"RMT_INTMASK= 0x%x, 0x%x" ,transq[id]->RMT_INTMASK ,transq[id]->RMT_INTMASK_E);
    return HAL_TRANSQ_RET_OK;
}

enum HAL_TRANSQ_RET_T hal_transq_open(enum HAL_TRANSQ_ID_T id, const struct HAL_TRANSQ_CFG_T *cfg)
{
    const struct HAL_TRANSQ_SLOT_NUM_T *slot;
    uint32_t ctrl;
    enum HAL_TRANSQ_RET_T ret;

    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }
    if (cfg == NULL) {
        return HAL_TRANSQ_RET_BAD_CFG;
    }

    hal_cmu_clock_enable(transq_mod[id]);
    hal_cmu_reset_clear(transq_mod[id]);

    transq_cfg[id] = *cfg;

    slot = &cfg->slot;

    ret = hal_transq_update_num(id, slot);
    if (ret) {
        return ret;
    }

    next_tx_slot[id][HAL_TRANSQ_PRI_NORMAL] = slot->tx_num[HAL_TRANSQ_PRI_NORMAL] ? 0 :  transq_slot_num[id];
    active_tx_slot[id][HAL_TRANSQ_PRI_NORMAL] =  transq_slot_num[id];
    tx_slot_full[id][HAL_TRANSQ_PRI_NORMAL] = false;
    next_rx_slot[id][HAL_TRANSQ_PRI_NORMAL] = slot->rx_num[HAL_TRANSQ_PRI_NORMAL] ? 0 :  transq_slot_num[id];

    next_tx_slot[id][HAL_TRANSQ_PRI_HIGH] = slot->tx_num[HAL_TRANSQ_PRI_HIGH] ?
        ( transq_slot_num[id] - slot->tx_num[HAL_TRANSQ_PRI_HIGH]) :  transq_slot_num[id];
    active_tx_slot[id][HAL_TRANSQ_PRI_HIGH] =  transq_slot_num[id];
    tx_slot_full[id][HAL_TRANSQ_PRI_HIGH] = false;
    next_rx_slot[id][HAL_TRANSQ_PRI_HIGH] = slot->rx_num[HAL_TRANSQ_PRI_HIGH] ?
        ( transq_slot_num[id] - slot->rx_num[HAL_TRANSQ_PRI_HIGH]) :  transq_slot_num[id];

    transq[id]->RMT_ISC.RMT_INTCLR = ~0UL;
    transq[id]->LDONE_ISC.LDONE_INTCLR = ~0UL;
    transq[id]->LERR_ISC.LERR_INTCLR = ~0UL;

    ctrl = CTRL_REMOTE_IRQ_EN | CTRL_LOCAL_ERR_IRQ_EN;
    if (cfg->tx_handler) {
        ctrl |= CTRL_LOCAL_DONE_IRQ_EN;
    }
    transq[id]->CTRL = ctrl;

    if (cfg->rx_handler) {
        NVIC_SetVector(remote_irq_num[id], (uint32_t)hal_transq_remote_irq_handler);
        NVIC_SetPriority(remote_irq_num[id], IRQ_PRIORITY_NORMAL);
        NVIC_ClearPendingIRQ(remote_irq_num[id]);
        NVIC_EnableIRQ(remote_irq_num[id]);
    }

    NVIC_SetVector(local_irq_num[id], (uint32_t)hal_transq_local_irq_handler);
    NVIC_SetPriority(local_irq_num[id], IRQ_PRIORITY_NORMAL);
    NVIC_ClearPendingIRQ(local_irq_num[id]);
    NVIC_EnableIRQ(local_irq_num[id]);
	
  /*  TRACE(1,"hal_transq_open= %d, %d,%d,%d,%d,%d" , 
		next_tx_slot[id][HAL_TRANSQ_PRI_NORMAL],
		active_tx_slot[id][HAL_TRANSQ_PRI_NORMAL] ,
		next_rx_slot[id][HAL_TRANSQ_PRI_NORMAL],
		next_tx_slot[id][HAL_TRANSQ_PRI_HIGH],
		active_tx_slot[id][HAL_TRANSQ_PRI_HIGH],
		next_rx_slot[id][HAL_TRANSQ_PRI_HIGH] 

	);
	TRACE(1,"hal_transq_open, rx_handler 0x%08x, tx_handler 0x%08x" , 
		   transq_cfg[id].rx_handler, transq_cfg[id].tx_handler
	
	   );*/

    return HAL_TRANSQ_RET_OK;
}

enum HAL_TRANSQ_RET_T hal_transq_close(enum HAL_TRANSQ_ID_T id)
{
    if (id >= HAL_TRANSQ_ID_QTY) {
        return HAL_TRANSQ_RET_BAD_ID;
    }

    NVIC_DisableIRQ(remote_irq_num[id]);
    NVIC_DisableIRQ(local_irq_num[id]);

    transq[id]->CTRL = 0;

    hal_cmu_reset_set(transq_mod[id]);
    hal_cmu_clock_disable(transq_mod[id]);

    return HAL_TRANSQ_RET_OK;
}
enum HAL_TRANSQ_RET_T hal_transq_flush(enum HAL_TRANSQ_ID_T id)
{
    transq[id]->RMT_ISC.RMT_INTCLR = ~0UL;

    return HAL_TRANSQ_RET_OK;
}
uint8_t get_rx_irq_count(enum HAL_TRANSQ_ID_T id)
{
    return transq_cfg[id].rx_irq_count;
}

/* wifi specific */
struct wsm_hdr_t {
	uint16_t len;
	uint16_t id;
};

void show_reg(void)
{
    struct wsm_hdr_t * wsm;
    TRACE(0, " peer_transq[0]->RMT_MIS: 0x%08x, RMT_RIS: 0x%08x, RMT_INTMASK: 0x%08x, slot=%d ", 
        peer_transq[0]->RMT_MIS,
        peer_transq[0]->RMT_ISC.RMT_RIS,
        peer_transq[0]->RMT_INTMASK,
        next_rx_slot[0][0]);
    for(int i=0;i<32;i++)
    {
         wsm  = (struct wsm_hdr_t*) (peer_transq[0]->RSLOT[i].ADDR);
         if ((uint32_t)wsm >= 0x80000000 && (uint32_t)wsm <= 0x90000000)
            TRACE(0, "peer_transq[0]->RSLOT[%d].addr= 0x%08x, wsm->id: 0x%x, len= %d", i, (uint32_t)wsm, wsm->id,wsm->len);
         else
            TRACE(0, "error or unenabled transq msg. 0x%08x", (uint32_t)wsm);
    }
}
#endif // CHIP_HAS_TRANSQ
