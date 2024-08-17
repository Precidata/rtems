/*
 * $Id: bios_core.h 3764 2024-08-06 17:19:33Z cedric $
 *
 * Copyright (C) 2023 - 2024 Cedric Berger <cedric@berger.to>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * This file contains the minimum definitions needed to communicate
 * between the CM4 (bios) and CM7 (application code)
 */

#ifndef _BIOS_HSEM_H_
#define _BIOS_HSEM_H_

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifndef STM32H7_HSEM_COUNT
#define STM32H7_HSEM_COUNT 32
#endif

#ifdef __cplusplus
extern "C" {
#endif

    extern struct stm32h7_hsem_stats {
	uint32_t n_sem_irq_total;
	uint32_t n_sem_irq_unused;
	uint32_t n_sem_irq_direct;
	uint32_t n_sem_irq_server;
	uint32_t n_sem_irq_inflight;
	uint32_t n_server_handler;
	uint32_t n_add_direct_handler;
	uint32_t n_add_server_handler;
	uint32_t n_remove_handler;

    } stm32h7_hsem_stats;

    extern struct stm32h7_hsem_handler {
	enum {
	    STM32H7_HSEM_UNUSED = 0,
	    STM32H7_HSEM_DIRECT = 1,
	    STM32H7_HSEM_SERVER = 2,
	} mode;
	rtems_interrupt_handler handler;
	void *arg;
	rtems_interrupt_server_request request;
	bool inflight;
	uint32_t n_irq_calls;
	uint32_t n_server_handler;
    } stm32h7_hsem_handlers[STM32H7_HSEM_COUNT];

    int stm32h7_hsem_remove_handler(int semid);
    int stm32h7_hsem_add_irq_handler(int semid, rtems_interrupt_handler handler, void *arg);
    int stm32h7_hsem_add_server_handler(
	int semid, rtems_interrupt_handler handler, void *arg, int server_index);

#ifdef __cplusplus
}
#endif

#endif /* _BIOS_CORE_H_ */
