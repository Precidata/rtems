/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2023-2024 Cedric Berger <cedric@precidata.com>
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

#include <stm32h7/hal.h>
#include <stm32h7xx_ll_hsem.h>

#include <rtems.h>
#include <rtems/sysinit.h>

#include <sys/param.h>
#include <unistd.h>
#include <string.h>

#include <rtems/bspIo.h>

#include <stm32h7_hsem.h>

#ifndef STM32H7_HSEM_COUNT
#define STM32H7_HSEM_COUNT 32
#endif

struct stm32h7_hsem_stats stm32h7_hsem_stats;
struct stm32h7_hsem_handler stm32h7_hsem_handlers[STM32H7_HSEM_COUNT];

static bool stm32h7_hsem_init_done;

static void
stm32h7_hsem_irq_handler(void *arg)
{
    struct stm32h7_hsem_handler *h;
    uint32_t semid, m, cisr, clear = 0, disable = 0;
    stm32h7_hsem_stats.n_sem_irq_total++;

    cisr = HSEM->C1ISR;
    for (semid = 0, m = 1, h = stm32h7_hsem_handlers; semid < STM32H7_HSEM_COUNT;
	 semid++, m <<= 1, h++) {
	if (!(cisr & m))
	    continue;
	switch (h->mode) {
	case STM32H7_HSEM_UNUSED:
	default:
	    clear |= m;
	    stm32h7_hsem_stats.n_sem_irq_unused++;
	    break;
	case STM32H7_HSEM_DIRECT:
	    HSEM->C1ICR = m;
	    h->handler(h->arg);
	    h->n_irq_calls++;
	    stm32h7_hsem_stats.n_sem_irq_direct++;
	    break;
	case STM32H7_HSEM_SERVER:
	    disable |= m;
	    clear |= m;
	    if (!h->inflight) {
		rtems_interrupt_server_request_submit(&h->request);
		stm32h7_hsem_stats.n_sem_irq_server++;
	    } else {
		stm32h7_hsem_stats.n_sem_irq_inflight++;
	    }
	    h->inflight = 1;
	    h->n_irq_calls++;
	    break;
	}
    }
    if (disable)
	HSEM->C1IER &= ~disable;
    if (clear)
	HSEM->C1ICR = clear;
}

static void
stm32h7_hsem_server_handler(void *semarg)
{
    int semid = (int)(intptr_t)semarg;
    struct stm32h7_hsem_handler *h = stm32h7_hsem_handlers + semid;
    rtems_interrupt_level isr_cookie;
    uint32_t m = 1 << semid;

    if (semid < 0 || semid >= STM32H7_HSEM_COUNT)
	rtems_panic("stm32h7_hsem_server_handler: semaphore id: 0x%08x\n", semid);
    if (h->mode != STM32H7_HSEM_SERVER)
	rtems_panic("stm32h7_hsem_server_handler: semaphore %d mode %d\n", semid, h->mode);
    HSEM->C1ICR = m;
    if (h->handler != NULL)
	h->handler(h->arg);

    rtems_interrupt_disable(isr_cookie);
    h->inflight = 0;
    HSEM->C1IER |= m;
    stm32h7_hsem_stats.n_server_handler++;
    h->n_server_handler++;
    rtems_interrupt_enable(isr_cookie);
}

static void
stm32h7_hsem_init(void)
{
    stm32h7_hsem_init_done = TRUE;

    rtems_status_code status = rtems_interrupt_handler_install(
	HSEM1_IRQn, "hsem1", RTEMS_INTERRUPT_UNIQUE, stm32h7_hsem_irq_handler, NULL);
    if (status != RTEMS_SUCCESSFUL)
	rtems_panic("stm32h7_hsem_init: cannot install HSEM1_IRQn\n");
    printk("stm32h7_hsem_init: init\n");
}

int
stm32h7_hsem_remove_handler(int semid)
{
    struct stm32h7_hsem_handler *h;
    rtems_interrupt_level isr_cookie;
    int mask;

    if (semid < 0 || semid >= STM32H7_HSEM_COUNT)
	return (RTEMS_INVALID_NUMBER);
    h = stm32h7_hsem_handlers + semid;
    mask = 1 << semid;

    if (h->mode == STM32H7_HSEM_SERVER && h->inflight)
	rtems_interrupt_server_request_destroy(&h->request);

    rtems_interrupt_disable(isr_cookie);
    bzero(h, sizeof(*h));
    HSEM->C1IER &= ~mask;
    HSEM->C1ICR = mask;
    stm32h7_hsem_stats.n_remove_handler++;
    rtems_interrupt_enable(isr_cookie);

    return (RTEMS_SUCCESSFUL);
}

static int
stm32h7_hsem_do_add_handler(int semid, rtems_interrupt_handler handler, void *arg, int server_index)
{
    struct stm32h7_hsem_handler *h;
    rtems_interrupt_level isr_cookie;
    int mask;

    if (handler == NULL)
	return RTEMS_INVALID_ADDRESS;
    if (semid < 0 || semid >= STM32H7_HSEM_COUNT)
	return (RTEMS_INVALID_NUMBER);
    h = stm32h7_hsem_handlers + semid;
    mask = 1 << semid;

    if (h->mode != STM32H7_HSEM_UNUSED)
	return (RTEMS_RESOURCE_IN_USE);
    if (server_index >= 0) {
	int rv = rtems_interrupt_server_request_initialize(
	    server_index, &h->request, stm32h7_hsem_server_handler, (void *)semid);
	if (rv != RTEMS_SUCCESSFUL)
	    return (rv);
    }

    if (!stm32h7_hsem_init_done)
	stm32h7_hsem_init();

    rtems_interrupt_disable(isr_cookie);
    h->mode = (server_index >= 0) ? STM32H7_HSEM_SERVER : STM32H7_HSEM_DIRECT;
    h->handler = handler;
    h->arg = arg;
    h->inflight = 0;
    HSEM->C1ICR = mask;
    HSEM->C1IER |= mask;
    if (server_index >= 0)
	stm32h7_hsem_stats.n_add_server_handler++;
    else
	stm32h7_hsem_stats.n_add_direct_handler++;
    rtems_interrupt_enable(isr_cookie);

    return (RTEMS_SUCCESSFUL);
}

int
stm32h7_hsem_add_irq_handler(int semid, rtems_interrupt_handler handler, void *arg)
{
    int rv = stm32h7_hsem_do_add_handler(semid, handler, arg, -1);
    return (rv);
}

int
stm32h7_hsem_add_server_handler(
    int semid, rtems_interrupt_handler handler, void *arg, int server_index)
{
    if (server_index < 0)
	return (RTEMS_INVALID_ID);
    int rv = stm32h7_hsem_do_add_handler(semid, handler, arg, server_index);
    return (rv);
}
