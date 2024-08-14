/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2023 Karel Gardas
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

#include <rtems.h>
#include <bsp.h>
#include <stm32h7/memory.h>
#include <rtems/fatal.h>

#include <stddef.h>

/*
 * Support for custom app stack allocator. Usable for cases requiring
 * to have app stack in custom RAM specified. E.g. DTCM, SRAMx, AXI etc.
 * This trivial allocator does not support deallocation.
 */

#ifndef STM32H7_APP_STACK_IN_DEFAULT

#ifdef STM32H7_APP_STACK_IN_DTCM
#define STACK_LOCATION_BEGIN stm32h7_memory_dtcm_begin
#define STACK_LOCATION_SIZE stm32h7_memory_dtcm_size
#endif

#ifdef STM32H7_APP_STACK_IN_AXI
#define STACK_LOCATION_BEGIN stm32h7_memory_sram_axi_begin
#define STACK_LOCATION_SIZE stm32h7_memory_sram_axi_size
#endif

static size_t stm32h7_stack_allocate_counter;
static void  *stm32h7_stack_allocate_last_p;
static size_t stm32h7_stack_allocate_last_size;

void
stm32h7_stack_allocator_init(size_t size)
{
    /* nothing to be done here */
    stm32h7_stack_allocate_counter = 0;
}

void *
stm32h7_stack_allocate(size_t size)
{
    void *p;

    if (size + stm32h7_stack_allocate_counter > (size_t)STACK_LOCATION_SIZE) {
	printk("stack: cannot allocated %lu bytes (%lu used)\n",
	    size, stm32h7_stack_allocate_counter);
        return NULL;
    }
    p = STACK_LOCATION_BEGIN + stm32h7_stack_allocate_counter;
    stm32h7_stack_allocate_counter += size;

    /* remember last allocation */
    stm32h7_stack_allocate_last_p = p;
    stm32h7_stack_allocate_last_size = size;

    return (p);
}

void *
stm32h7_stack_idle_allocate(uint32_t cpu, size_t *psize)
{
    return (stm32h7_stack_allocate(*psize));
}

void
stm32h7_stack_deallocate(void* p)
{
    if (p == stm32h7_stack_allocate_last_p) {
	stm32h7_stack_allocate_last_p = NULL;
	stm32h7_stack_allocate_counter -= stm32h7_stack_allocate_last_size;
#if 0
        printk("stack: deallocate last stack at %p (%lu bytes)\n", p,
            stm32h7_stack_allocate_last_size);
#endif
    } else
	printk("stack: cannot deallocate %p\n", p);
}

#endif /* !STM32H7_APP_STACK_IN_DEFAULT */
