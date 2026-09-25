/**
******************************************************************************
* @file    stm32c5a3xg_flash.ld
* @brief   Linker script for STM32C5A3ZGT6 on NUCLEO-C5A3ZG
*
*          *** PROJECT-CUSTOMISED - DO NOT REGENERATE ***
*
*          This file started life as the STM32CubeC5 DFP template
*          (stm32c5xx_dfp/Source/Templates/gcc/linker/stm32c5a3xg_flash.ld)
*          but it is no longer the stock script. The deviations below are
*          load-bearing: application code, the startup copy table and the
*          the test fixture's memory-placement contracts depend on them.
*
*            1. The last flash region (2 KiB @ 0x080FF800) is carved out of ROM
*               into its own CFG region and holds the .app_config blob. The
*               test config lives at that fixed address. It must NOT move.
*               This is a logical region, NOT an assertion of hardware erase
*               granularity. The fixture contains no flash-write/erase routine.
*
*            2. A .ramfunc output section is provided for code that has to
*               execute out of SRAM (flash-write helpers, timing-critical
*               update paths). It is loaded from ROM and copied to RAM by the
*               CMSIS startup copy table, so any new entry added to
*               .copy.table must keep the {src, dst, wordcount} triplet shape.
*
*            3. The last 1 KiB of SRAM (0x2003FC00) is a NORETAIN region
*               holding .noinit. It is deliberately absent from .zero.table so
*               that the diagnostic breadcrumbs written there survive a warm
*               reset. Zeroing it would destroy the reset-reason trail.
*
*            4. HEAP_SIZE and STACK_SIZE reserve 2 KiB and 4 KiB respectively
*               for the application's future integration needs.
*
*          Region sizes are asserted at the bottom of this file. If a link
*          fails there, fix the code - do not relax the ASSERT.
*
* Copyright (c) 2026 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Raised from the 0x200 / 0x400 template defaults - see note 4 above. */
HEAP_SIZE  = 0x800;
STACK_SIZE = 0x1000;

/* Board identity / calibration page - see note 1 above. */
APP_CONFIG_SIZE = 0x800;

MEMORY
{
  /* 1 MiB flash, minus the config page carved off the top. */
  ROM      (rx)  : org = 0x08000000, len = 0x100000 - APP_CONFIG_SIZE
  CFG      (r)   : org = 0x080FF800, len = APP_CONFIG_SIZE

  /* 256 KiB SRAM, minus the retained breadcrumb page at the top. */
  RAM      (xrw) : org = 0x20000000, len = 0x3FC00
  NORETAIN (xrw) : org = 0x2003FC00, len = 0x400
}

/* Separate executable RAM code from writable data in the ELF load segments.
 * These flags describe the image; MCU access policy is configured by the MPU. */
PHDRS
{
  rom PT_LOAD FLAGS(5);
  config PT_LOAD FLAGS(4);
  ramcode PT_LOAD FLAGS(5);
  ramdata PT_LOAD FLAGS(6);
  retained PT_LOAD FLAGS(6);
}

SECTIONS
{
  .vectors :
  {
    . = ALIGN(8);
    KEEP(*(.vectors));
    . = ALIGN(8);
  } > ROM :rom

  .text :
  {
    . = ALIGN(8);
    *(.text);
    *(.text*);
    *(.glue_7);
    *(.glue_7t);
    *(.eh_frame);
    KEEP (*(.init));
    KEEP (*(.fini));
    . = ALIGN(8);
    _etext = .;
  } > ROM

  .rodata :
  {
    . = ALIGN(8);
    *(.rodata);
    *(.rodata*);
    . = ALIGN(8);
  } > ROM

  .ARM.extab (READONLY) :
  {
    . = ALIGN(8);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(8);
  } > ROM

  .ARM (READONLY) :
  {
    . = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*);
    __exidx_end = .;
    . = ALIGN(8);
  } > ROM

  .preinit_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*));
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(8);
  } > ROM

  .init_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)));
    KEEP (*(.init_array*));
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(8);
  } > ROM

  .fini_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)));
    KEEP (*(.fini_array*));
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(8);
  } > ROM

  /* ---------------------------------------------------------------------
   * Board identity / calibration page. Fixed address, its own flash page so
   * the factory tool can rewrite it without touching the application image.
   * KEEP() is required: nothing in the image references it at link time.
   * ------------------------------------------------------------------- */
  .app_config :
  {
    . = ALIGN(8);
    __app_config_start__ = .;
    KEEP (*(.app_config));
    KEEP (*(.app_config*));
    . = ALIGN(8);
    __app_config_end__ = .;
  } > CFG :config

  /* ---------------------------------------------------------------------
   * Startup copy table. Each entry is {load address, run address, words}.
   * .ramfunc is copied alongside .data by __cmsis_start().
   * ------------------------------------------------------------------- */
  .copy.table (READONLY) :
  {
    . = ALIGN(8);
    __copy_table_start__ = .;

    LONG (LOADADDR(.data));
    LONG (ADDR(.data));
    LONG (SIZEOF(.data) / 4);

    LONG (LOADADDR(.ramfunc));
    LONG (ADDR(.ramfunc));
    LONG (SIZEOF(.ramfunc) / 4);

    __copy_table_end__ = .;
  } > ROM :rom

  /* ---------------------------------------------------------------------
   * Zero table. NOTE: .noinit is deliberately NOT listed here - see note 3.
   * ------------------------------------------------------------------- */
  .zero.table (READONLY) :
  {
    . = ALIGN(8);
    __zero_table_start__ = .;
    LONG (ADDR(.bss));
    LONG (SIZEOF(.bss) / 4);
    __zero_table_end__ = .;
  } > ROM :rom

  /* ---------------------------------------------------------------------
   * Code that must run from SRAM. Mark functions with RAMFUNC (board.h).
   * ------------------------------------------------------------------- */
  .ramfunc :
  {
    . = ALIGN(8);
    __ramfunc_start__ = .;
    *(.ramfunc);
    *(.ramfunc*);
    . = ALIGN(8);
    __ramfunc_end__ = .;
  } > RAM AT> ROM :ramcode

  .data :
  {
    . = ALIGN(8);
    _sidata = LOADADDR(.data);
    __data_start__ = .;
    _sdata = .;
    *(.data);
    *(.data*);
    . = ALIGN(8);
    _edata = .;
  } > RAM AT> ROM :ramdata

  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss);
    *(.bss*);
    *(COMMON);
    . = ALIGN(8);
    _ebss = .;
    __bss_end__ = _ebss;
  } > RAM

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE (end = .);
    _heap_start = .;
    . += HEAP_SIZE;
    . = ALIGN(8);
    _heap_end = .;
    __HeapLimit = .;
  } > RAM

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
    _estack = .;
    __stack = .;
  } > RAM

  /* ---------------------------------------------------------------------
   * Retained across warm reset. Never zeroed by startup - see note 3.
   * ------------------------------------------------------------------- */
  .noinit (NOLOAD) :
  {
    . = ALIGN(8);
    __noinit_start__ = .;
    *(.noinit);
    *(.noinit*);
    . = ALIGN(8);
    __noinit_end__ = .;
  } > NORETAIN :retained

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 :
  {
    *(.ARM.attributes)
  }
}

/* The factory tool writes this page; the image must not spill past it. */
ASSERT(__app_config_end__ - __app_config_start__ <= APP_CONFIG_SIZE,
       "ERROR: .app_config overflows the reserved flash config page")

/* Breadcrumbs must fit the retained page, or a warm reset loses them. */
ASSERT(__noinit_end__ - __noinit_start__ <= 0x400,
       "ERROR: .noinit overflows the retained SRAM page")
