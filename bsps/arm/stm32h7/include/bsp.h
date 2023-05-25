/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2020 embedded brains GmbH & Co. KG
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

#ifndef LIBBSP_ARM_STM32H7_BSP_H
#define LIBBSP_ARM_STM32H7_BSP_H

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSBSPsARMSTM32H7 STM32H7
 *
 * @ingroup RTEMSBSPsARM
 *
 * @brief STM32H7 Board Support Package.
 *
 * @{
 */

#define BSP_FEATURE_IRQ_EXTENSION

#define BSP_ARMV7M_IRQ_PRIORITY_DEFAULT (13 << 4)

#define BSP_ARMV7M_SYSTICK_PRIORITY (14 << 4)

#define BSP_ARMV7M_SYSTICK_FREQUENCY stm32h7_systick_frequency()

uint32_t stm32h7_systick_frequency(void);

/* default functions */
void stm32h7_init_power(void);
void stm32h7_init_oscillator(void);
void stm32h7_init_clocks(void);
void stm32h7_init_peripheral_clocks(void);
void stm32h7_init_qspi(void);

/** @} */

#if !defined(STM32H7_APP_STACK_IN_DEFAULT)

void  stm32h7_stack_allocator_init(size_t);
void* stm32h7_stack_allocate(size_t);
void* stm32h7_stack_idle_allocate(uint32_t, size_t*);
void  stm32h7_stack_deallocate(void*);

#define CONFIGURE_TASK_STACK_ALLOCATOR_AVOIDS_WORK_SPACE 1
#define CONFIGURE_TASK_STACK_ALLOCATOR_INIT stm32h7_stack_allocator_init
#define CONFIGURE_TASK_STACK_ALLOCATOR stm32h7_stack_allocate
#define CONFIGURE_TASK_STACK_DEALLOCATOR stm32h7_stack_deallocate
#define CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE stm32h7_stack_idle_allocate

#endif /* ! STM32H7_APP_STACK_IN_DEFAULT */

#ifdef __cplusplus
}
#endif

#endif /* LIBBSP_ARM_STM32H7_BSP_H */
