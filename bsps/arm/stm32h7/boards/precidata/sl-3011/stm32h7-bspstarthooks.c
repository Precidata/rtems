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

void bsp_start_hook_0(void)
{
    BIOS->cm7.magic = 0xfacade01;
    BIOS->cm7.state = BIOS_CORE_RUNNING;
    BIOS->cm7.updates++;

  bios_inst_led_solid(BIOS_LED_WHITE);
  bios_console_write("Hello\n", 6);
  bios_vtty_write(0, "Hello\r\n", 7);
  /* Let's setup core clock variable by calling update function here.
     This should be actually the only system/hw init we need to perform on
     slave side. */
  //  SystemCoreClock = 300000000;
  SystemCoreClockUpdate();
#if __CORTEX_M == 0x07U
  if ((SCB->CCR & SCB_CCR_IC_Msk) == 0) {
    SCB_EnableICache();
  }

  if ((SCB->CCR & SCB_CCR_DC_Msk) == 0) {
    SCB_EnableDCache();
  }
  /* not using ARMV7M_MPU_CTRL_DEFAULT here to avoid ARMV7M_MPU_CTRL_PRIVDEFENA */
  _ARMV7M_MPU_Setup(ARMV7M_MPU_CTRL_ENABLE, stm32h7_config_mpu_region, stm32h7_config_mpu_region_count);
#endif
}

static void fwlog(const char *s) {
    bios_console_write(s, strlen(s));
}

void bsp_start_hook_1(void)
{
    BIOS->cm7.updates++;
  bios_inst_led_solid(BIOS_LED_CYAN);
  fwlog("Before copy sections...\n");
  bsp_start_copy_sections_compact();
  fwlog("After copy sections.\n");
#if __CORTEX_M == 0x07U
  SCB_CleanDCache();
  SCB_InvalidateICache();
#endif
  bsp_start_clear_bss();
    BIOS->cm7.state = BIOS_CORE_RUNNING;
    BIOS->cm7.updates++;
}
