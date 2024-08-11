/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2020 embedded brains GmbH (http://www.embedded-brains.de)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <bsp.h>
#include <bsp/bootcard.h>
#include <bsp/linker-symbols.h>
#include <bsp/start.h>
#include <stm32h7/hal.h>
#include <stm32h7/memory.h>
#include <stm32h7/mpu-config.h>
#include <rtems/score/armv7m.h>

#include <bspopts.h>

#include <string.h>

#include <bios_core.h>

uint32_t slave_wait_cycles = 0;

static void MinimalSystemInit (void)
{
  /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
#endif

  /*SEVONPEND enabled so that an interrupt coming from the CPU(n) interrupt signal is
    detectable by the CPU after a WFI/WFE instruction.*/
  SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

  SCB->VTOR = (uint32_t) bsp_start_vector_table_begin;
}

void bsp_start_hook_0(void)
{
  MinimalSystemInit();

  for (;;) {
    __DSB();
    if (BIOS->cm4.magic == 0xfacade01 &&
      BIOS->cm4.state != BIOS_CORE_INIT &&
      BIOS->cm4.updates > 0)
      break; /* cm4 is running */
  }

  SystemCoreClockUpdate();
  HAL_Init();

#if __CORTEX_M == 0x07U
  if ((SCB->CCR & SCB_CCR_IC_Msk) == 0) {
    SCB_EnableICache();
  }

  if ((SCB->CCR & SCB_CCR_DC_Msk) == 0) {
    SCB_EnableDCache();
  }
  /* not using ARMV7M_MPU_CTRL_DEFAULT here to avoid ARMV7M_MPU_CTRL_PRIVDEFENA */
  _ARMV7M_MPU_Setup(ARMV7M_MPU_CTRL_ENABLE, stm32h7_config_mpu_region, stm32h7_config_mpu_region_count);
  SCB_InvalidateICache();
  SCB_CleanInvalidateDCache();
#endif
}

extern void getentropy_hook(void);

void bsp_start_hook_1(void)
{
  bsp_start_copy_sections_compact();
#if __CORTEX_M == 0x07U
  SCB_CleanDCache();
  SCB_InvalidateICache();
#endif
  bsp_start_clear_bss();

  bios_set_state(BIOS_CORE_RUNNING);
  bios_console_write("rtems: running\n", 15);

#ifdef STM32H7_SLAVE_BSP_HW_ETH
  stm32h7_clk_enable(STM32H7_MODULE_RNG);
  getentropy_hook();
#endif
}
