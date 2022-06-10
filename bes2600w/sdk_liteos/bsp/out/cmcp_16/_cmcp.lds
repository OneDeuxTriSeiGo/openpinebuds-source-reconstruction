/***************************************************************************
 *
 * Copyright 2015-2019 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/
/***************************************************************************
 *
 * Copyright 2015-2020 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/
/***************************************************************************
 *
 * Copyright 2015-2019 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/
/* total ramv size is 0x30000 */
/*MCU use RAM0, CP and CPX use RAM1*/
/* secure and non-secure ram size */
/* For linker scripts */
/* For module features */
/* For boot struct version */
/* For ROM export functions */
/* Linker script to configure memory regions. */
MEMORY
{
 RAMCP (rwx) : ORIGIN = 0x20000000+0x180000-0x40000+0x30000-0x80000, LENGTH = 0x40000-0x30000
 RAMCPX (rwx) : ORIGIN = 0x00200000+0x180000-0x40000+0x30000-0x80000, LENGTH = 0x40000-0x30000
 PSRAMCP (rwx) : ORIGIN = (0x34000000 + 0x800000-0x100000), LENGTH = 0x100000
 PSRAMCPX (rx) : ORIGIN = (0x14000000 + 0x800000-0x100000), LENGTH = 0x100000
 RAMV (rwx) : ORIGIN = 0x20200000, LENGTH = 0x30000
}
/* Library configurations */
GROUP(libgcc.a)
/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions ROM and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __cp_etext
 *   __cp_data_start__
 *   __cp_data_end__
 *   __cp_bss_start__
 *   __cp_bss_end__
 *   __cp_HeapLimit
 *   __cp_StackLimit
 *   __cp_StackTop
 *   __cp_stack
 *   __Vectors_End
 *   __Vectors_Size
 *   __free_cp_ram
 */
ENTRY(Reset_Handler)
SECTIONS
{
 .cp_text (ORIGIN(PSRAMCPX)) :
 {
  _stext = .;
  KEEP(*(.vectors))
  *(.text*)
  *(.cp_text_sram*)
  KEEP(*(.init))
  KEEP(*(.fini))
  /* .ctors */
  *crtbegin.o(.ctors)
  *crtbegin?.o(.ctors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
  *(SORT(.ctors.*))
  *(.ctors)
  /* .dtors */
  *crtbegin.o(.dtors)
  *crtbegin?.o(.dtors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
  *(SORT(.dtors.*))
  *(.dtors)
  . = ALIGN(4);
  *(.rodata*)
  KEEP(*(.eh_frame*))
  KEEP(*(.build_info))
  *(.note.gnu.build-id)
  _etext = .;
  . = ALIGN(4);
 } > PSRAMCPX
 .cp_got_info (.) :
 {
  __cp_got_info_start = .;
  __cp_got_start = .;
  *(.got)
  . = ALIGN(4);
  __cp_got_end = .;
  __cp_got_plt_start = .;
  *(.got.plt)
  . = ALIGN(4);
  __cp_igot_plt_start = .;
  *(.igot.plt)
  . = ALIGN(4);
  __cp_dynamic_start = .;
  *(.dynamic)
  . = ALIGN(4);
  __cp_got_info_end = .;
 } > PSRAMCPX
 .ARM.extab (.) :
 {
  *(.ARM.extab* .gnu.linkonce.armextab.*)
 } > PSRAMCPX
 __exidx_start = .;
 .ARM.exidx (.) :
 {
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
 } > PSRAMCPX
 __exidx_end = .;
 . = ALIGN(4);
 __cp_etext = .;
 _eronly = .;
 .cp_data (((.) - 0x14000000 + 0x34000000)) : AT (__cp_etext)
 {
  __cp_data_start__ = .;
  _sdata = .;
  *(.data*)
  . = ALIGN(4);
  *(vtable)
  . = ALIGN(4);
  /* preinit data */
  PROVIDE_HIDDEN (__preinit_array_start = .);
  KEEP(*(.preinit_array))
  PROVIDE_HIDDEN (__preinit_array_end = .);
  . = ALIGN(4);
  /* init data */
  PROVIDE_HIDDEN (__init_array_start = .);
  KEEP(*(SORT(.init_array.*)))
  KEEP(*(.init_array))
  PROVIDE_HIDDEN (__init_array_end = .);
  . = ALIGN(4);
  /* finit data */
  PROVIDE_HIDDEN (__fini_array_start = .);
  KEEP(*(SORT(.fini_array.*)))
  KEEP(*(.fini_array))
  PROVIDE_HIDDEN (__fini_array_end = .);
  KEEP(*(.jcr*))
  . = ALIGN(4);
  _edata = .;
  /* All data end */
  __cp_data_end__ = .;
 } > PSRAMCP
 .vector_table_cp (.) (NOLOAD) :
 {
  . = ALIGN(0x200);
  __cp_vector_start__ = .;
  KEEP(*(.vector_table_cp))
  . = __cp_vector_start__ + 360;
  . = ALIGN(4);
 } > PSRAMCP
 .cp_bss (.) (NOLOAD) : AT (ADDR(.cp_bss))
 {
  . = ALIGN(4);
  _sbss = .;
  __cp_bss_start__ = .;
  *(.bss*)
  *(.cp_bss*)
  *(COMMON)
                . = ALIGN(64);
                __os_heap_start__ = .;
                m_aucSysMem0 = .;
                . += 0x40000;
                __os_heap_end__ = .;
  . = ALIGN(4);
  __cp_bss_end__ = .;
  _ebss = .;
  __bss_start__ = __cp_bss_start__;
  __bss_end__ = __cp_bss_end__;
  . = ALIGN(4);
 } > PSRAMCP
 .cp_heap (.) (NOLOAD) :
 {
  . = ALIGN(8);
  __cp_HeapBase = .;
  __end__ = .;
  end = __end__;
  . += 0x1000;
  . = ALIGN(8);
  __cp_HeapLimit = .;
 } > PSRAMCP
 /* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
 .cp_stack_dummy (.) (COPY) :
 {
  . = 0x1000;
  . = ALIGN(8);
 } > PSRAMCP
 /* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
 __cp_StackTop = ORIGIN(PSRAMCP) + LENGTH(PSRAMCP);
 __cp_StackLimit = __cp_StackTop - SIZEOF(.cp_stack_dummy);
 PROVIDE(__cp_stack = __cp_StackTop);
 __StackLimit = __cp_StackLimit;
 __StackTop = __cp_StackTop;
 PROVIDE(__stack = __cp_StackTop);
 _sstack = __cp_StackLimit;
 _estack = __cp_StackTop;
 /* Check if data + heap + stack exceeds RAM limit */
 ASSERT(__cp_StackLimit >= __cp_HeapLimit, "region RAM overflowed with stack")
 __free_cp_ram = __cp_StackLimit - __cp_HeapLimit;
 __cp_text_data_end = __cp_etext + SIZEOF(.cp_data);
 .cp_sram_bss (ORIGIN(RAMCP)) (NOLOAD) : AT (ADDR(.cp_sram_bss))
 {
  . = ALIGN(4);
  *(.sram_bss*)
 } > RAMCP
 .cp_vram_bss (ORIGIN(RAMV)) (NOLOAD) : AT (ADDR(.cp_vram_bss))
 {
  . = ALIGN(4);
  *(.vram_bss*)
 } > RAMV
}
