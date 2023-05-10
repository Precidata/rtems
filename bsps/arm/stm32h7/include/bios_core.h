/*
 * $Id: bios_core.h 3376 2023-05-07 21:11:04Z cedric $
 *
 * Copyright (C) 2023 - 2023 Cedric Berger <cedric@berger.to>
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

#ifndef _BIOS_CORE_H_
#define _BIOS_CORE_H_

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIOS_MAGIC_V1		0xfacade01

#ifndef BIOS_INLINE
#define BIOS_INLINE		static inline __attribute__((always_inline))
#define _BIOS_INLINE_DEFINED_
#endif

#ifndef BIOS_SRAM4
#define BIOS_SRAM4		__attribute__((section(".sram4")))
#endif

#define BIOS_CACHE_ALIGNED	__attribute__((aligned(32)))

/*
 * BIOS root structure is located at beginning of SRAM4
 */
#define BIOS			((struct bios_root *)0x38000000)

/* memory barrier */
#define BIOS_DMB()		asm volatile ("dmb" : : : "memory")
#define BIOS_DSB()		asm volatile ("dsb" : : : "memory")
#define BIOS_ISB()		asm volatile ("isb" : : : "memory")
#define BIOS_WAITING_RETRY	100000
#define BIOS_COMMAND_RETRY	1000000

/* cache control */
#define BIOS_SCB_BASE           0xe000ed00UL
#define BIOS_SCB_CCR           	(*(volatile uint32_t *)(BIOS_SCB_BASE+0x014))
#define BIOS_SCB_CCSIDR         (*(volatile uint32_t *)(BIOS_SCB_BASE+0x080))
#define BIOS_SCB_CSSELR         (*(volatile uint32_t *)(BIOS_SCB_BASE+0x084))
#define BIOS_SCB_ICIALLU        (*(volatile uint32_t *)(BIOS_SCB_BASE+0x250))
#define BIOS_SCB_DCIMVAC        (*(volatile uint32_t *)(BIOS_SCB_BASE+0x25C))
#define BIOS_SCB_DCISW          (*(volatile uint32_t *)(BIOS_SCB_BASE+0x260))
#define BIOS_SCB_DCCMVAC        (*(volatile uint32_t *)(BIOS_SCB_BASE+0x268))

#define BIOS_DCACHE_ENABLED	(BIOS_SCB_CCR & (1<<16))
#define BIOS_DCACHE_LINE	32

/*
 * LED colors
 */
typedef enum bios_led_color {
    BIOS_LED_BLACK		= 0x0000,
    BIOS_LED_RED		= 0x0001,
    BIOS_LED_GREEN		= 0x0002,
    BIOS_LED_YELLOW		= 0x0003,
    BIOS_LED_BLUE		= 0x0004,
    BIOS_LED_MAGENTA		= 0x0005,
    BIOS_LED_CYAN		= 0x0006,
    BIOS_LED_WHITE		= 0x0007,
    BIOS_LED_MAX,
} bios_led_color;

typedef enum bios_core_state {
    /* for use by BIOS only */
    BIOS_CORE_INIT		= 0,	/* initial state, core not working */
    BIOS_CORE_WAITING		= 1,	/* synchronizing with the other core */

    /* for use by BIOS and app */
    BIOS_CORE_STARTING		= 2,	/* running initialization code */
    BIOS_CORE_RUNNING		= 3,	/* running application code */
    BIOS_CORE_STOPPING		= 4,	/* shutting down */
    BIOS_CORE_SUCCESS		= 5,	/* exited with success */
    BIOS_CORE_FAILURE		= 6,	/* exited with failure */
    BIOS_CORE_CRASHED		= 7,	/* exited after a crash */
} bios_core_state;


/********** structures **********/

/*
 * Version number of the software. Little endian.
 * build:
 *    0x00-0x3f: alpha versions
 *    0x40-0x7f: beta versions
 *    0x80-0xbf: release candidates
 *    0xc0-0xff: released versions
 */
typedef union bios_version {
    uint32_t			 u32;
    struct {
	uint8_t			 build;
	uint8_t			 micro;
	uint8_t			 minor;
	uint8_t			 major;
    };
} bios_version;

/*
 * Basic information about the software running on one core
 */
typedef struct bios_core_info {
    /*
     * The magic word, 0xfacade01, is only set when the other fields
     * are valid. Furthermore, every 1ms at least, 'updates' must be
     * incremented and the 'magic' and 'state' fields must be rewritten.
     */
    volatile uint32_t		 magic;
    volatile bios_core_state	 state;
    volatile uint32_t		 updates;
    bios_version		 version;
} bios_core_info;

/*
 * Unidirectional ring buffer for byte elements
 */
typedef struct bios_byte_queue {
    uint8_t			*buf;	 /* ring buffer */
    uint32_t			 size;	 /* number of bytes, power of 2 */
    volatile uint32_t		*head;	 /* read pointer */
    volatile uint32_t		*tail;	 /* write pointer */
} bios_byte_queue;

/*
 * Virtual TTY, to connect to a shell running on the CM7
 */
typedef struct bios_virtual_tty {
    struct bios_byte_queue	*inq;	 /* CM4 => CM7 */
    struct bios_byte_queue	*outq;	 /* CM7 => CM4 */
    bool			 connected;	/* CM4 => CM7 */
} bios_virtual_tty;

/*
 * RPC calls between M4 and M7 cores
 */

/*
 * Argument or return value for the RPC call
 */
typedef union bios_rpc_arg {
    uint32_t			 u32;	 /* unsigned 32-bit quantity */
    int32_t			 i32;	 /* signed 32-bit quantity */
    float			 f32;	 /* 32-bit floating point */
    void			*p32;	 /* 32-bit pointer or string */
} bios_rpc_arg;

/*
 * Command to be executed by the other core.
 */
typedef enum bios_rpc_command {
    BIOS_CMD_NONE		= 0,
    BIOS_CMD_RUN		= 1,
    BIOS_CMD_TRAP		= 2,
    BIOS_CMD_MEMSET_32		= 10,
    BIOS_CMD_MEMCPY_32		= 11,
    BIOS_CMD_ETH_PACKET		= 20,
    BIOS_CMD_MAX
} bios_rpc_command;

/*
 * State of the RPC call. written by the slave.
 * Failures are represented by negative values, -errno
 */
typedef enum bios_rpc_status {
    BIOS_RPC_IDLE		= 0,
    BIOS_RPC_RUNNING		= 1,	/* command running */
    BIOS_RPC_SUCCESS		= 2,	/* command success */
} bios_rpc_status;

/*
 * Simple command buffer, from one core to the other
 * Each direction is aligned 32-bytes for easy cache management
 */
typedef struct bios_rpc_buffer {
    volatile bios_rpc_command	 command;	/* master => slave */
    bios_rpc_arg		 param[7];	/* master => slave */
    volatile bios_rpc_status	 status;	/* slave => master */
    bios_rpc_arg		 retval[7];	/* slave => master */
} bios_rpc_buffer BIOS_CACHE_ALIGNED;
static_assert(sizeof(bios_rpc_buffer) == 64);

/*
 * Root BIOS structure, located at the beginning of SRAM4
 *
 * The devid must be written by the CM7 ASAP because the
 * CM4 has no access to the device electronic signature
 * memory map located at 0x1ff1e800. See bios_init_devid()
 */
typedef struct bios_root {
    bios_core_info		 cm4;		/* CM4 => CM7 */
    bios_core_info		 cm7;		/* CM7 => CM4 */
    uint8_t			 iled[4];	/* CM7 => CM4 */
    bios_byte_queue		*console;	/* initialized by CM4 */
    void		       (*exit)(int);	/* exit function */
    uint32_t			 nvtty;		/* number of virtual ttys */
    bios_virtual_tty		*vttys;		/* list of virtual ttys */
    uint32_t			 nrpc4to7;	/* number of command bloc, m4 => m7 */
    bios_rpc_buffer		*rpc4to7;	/* command bloc, m4 => m7 */
    uint32_t			 nrpc7to4;	/* number of command bloc, m7 => m4 */
    bios_rpc_buffer		*rpc7to4;	/* command bloc, m7 => m4 */
} bios_root;


/********** functions **********/

/*
 * Perform a DMB and sleep a little bit to avoid hammering the shared memory
 */
BIOS_INLINE void
bios_dmb_delay(void)
{
    /* sleep a bit */
    BIOS_DMB();
    for (volatile int i = 0; i < 16; i++)
	asm volatile ("nop");
}

/*
 * Update our state
 */
BIOS_INLINE void
bios_set_state(bios_core_state state)
{
    BIOS->cm7.updates++;
    BIOS->cm7.state = state;
    BIOS->cm7.magic = BIOS_MAGIC_V1;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
        BIOS_SCB_DCCMVAC = (intptr_t)&BIOS->cm7;
        BIOS_DSB();
        BIOS_ISB();
    }
}

/*
 * Make the INST LED to alternate over the 4 given colors, 250ms /B color.
 */
BIOS_INLINE void
bios_inst_led_blink(enum bios_led_color c1, enum bios_led_color c2,
    enum bios_led_color c3, enum bios_led_color c4)
{
    BIOS->iled[0] = (uint8_t)c1;
    BIOS->iled[1] = (uint8_t)c2;
    BIOS->iled[2] = (uint8_t)c3;
    BIOS->iled[3] = (uint8_t)c4;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED)
	BIOS_SCB_DCCMVAC = (intptr_t)BIOS->iled;
    /* no need to wait here */
}

/*
 * Make the INST LED to display a solid color.
 */
BIOS_INLINE void
bios_inst_led_solid(enum bios_led_color c)
{
    bios_inst_led_blink(c, c, c, c);
}

/*
 * Lookup available size on write buffer
 */
BIOS_INLINE int
bios_bqe_write_available(bios_byte_queue *bq)
{
    uint32_t size = bq->size;
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	BIOS_SCB_DCIMVAC = (intptr_t)bq->head;
	BIOS_DSB();
	BIOS_ISB();
    }
    uint32_t head = *bq->head;
    uint32_t tail = *bq->tail;
    uint32_t used = tail - head;

    if (used > size)
	return (-EFAULT);		 /* corrupted queue */
    return (size - used);
}

/*
 * Write up to n bytes to the given byte queue.
 * Return -EAGAIN if the queue is full, and may to partial writes.
 * Error code (errno) is returned as a negative integer.
 */
BIOS_INLINE int
bios_bqe_write(bios_byte_queue *bq, const void *buf, unsigned n)
{
    uint32_t size = bq->size;
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	BIOS_SCB_DCIMVAC = (intptr_t)bq->head;
	BIOS_DSB();
	BIOS_ISB();
    }
    uint32_t head = *bq->head;
    uint32_t tail = *bq->tail;
    uint32_t used = tail - head;

    if (n > size)
	return (-EINVAL);		 /* message too big */
    if (used > size)
	return (-EFAULT);		 /* corrupted queue */
    if (used == size)
	return (-EAGAIN);		 /* queue full */
    if (n > size - used)
	n = size - used;		 /* partial write */
    uint32_t i = tail;
    uint32_t mask = size - 1;
    const uint8_t *p = (const uint8_t *)buf;
    for (unsigned j = 0; j < n; j++)
	bq->buf[i++ & mask] = *p++;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
	for (uint32_t i = 0; i < size; i += BIOS_DCACHE_LINE)
	    BIOS_SCB_DCCMVAC = (intptr_t)(bq->buf + i);
	BIOS_DSB();
	BIOS_ISB();
    }
    *bq->tail = i;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
	BIOS_SCB_DCCMVAC = (intptr_t)bq->tail;
	BIOS_DSB();
	BIOS_ISB();
    }
    return (n);
}

/*
 * Read up to n bytes from the given byte queue.
 * Return -EAGAIN if the queue is empty, and may to partial reads.
 * Error code (errno) is returned as a negative integer.
 */
BIOS_INLINE int
bios_bqe_read(bios_byte_queue *bq, void *buf, unsigned n)
{
    uint32_t size = bq->size;
    uint32_t head = *bq->head;
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	BIOS_SCB_DCIMVAC = (intptr_t)bq->tail;
	BIOS_DSB();
	BIOS_ISB();
    }
    uint32_t tail = *bq->tail;
    uint32_t used = tail - head;

    if (used > size)
	return (-EFAULT);		 /* corrupted queue */
    if (used == 0)
	return (-EAGAIN);		 /* queue empty */
    if (n > used)
	n = used;			 /* partial read */
    uint32_t i = head;
    uint32_t mask = size - 1;
    uint8_t *p = (uint8_t *)buf;
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	for (uint32_t i = 0; i < size; i += BIOS_DCACHE_LINE)
	    BIOS_SCB_DCIMVAC = (intptr_t)(bq->buf + i);
	BIOS_DSB();
	BIOS_ISB();
    }
    for (unsigned j = 0; j < n; j++)
	*p++ = bq->buf[i++ & mask];
    *bq->head = i;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
	BIOS_SCB_DCCMVAC = (intptr_t)bq->head;
	BIOS_DSB();
	BIOS_ISB();
    }
    return (n);
}

/*
 * Writes to the engineering debug console.
 */
BIOS_INLINE int
bios_console_write(const void *buf, unsigned n)
{
    return (bios_bqe_write(BIOS->console, buf, n));
}

/*
 * Writes to one of the virtual tty channels.
 */
BIOS_INLINE int
bios_vtty_write(unsigned tty, const void *buf, unsigned n)
{
    if (tty >= BIOS->nvtty)
	return (-ENXIO);
    return (bios_bqe_write(BIOS->vttys[tty].outq, buf, n));
}

/*
 * Reads from one of the virtual tty channels.
 */
BIOS_INLINE int
bios_vtty_read(unsigned tty, void *buf, unsigned n)
{
    if (tty >= BIOS->nvtty)
	return (-ENXIO);
    return (bios_bqe_read(BIOS->vttys[tty].inq, buf, n));
}

/*
 * Returns true if the virtual tty channel is in use by a client.
 */
BIOS_INLINE int
bios_vtty_is_connected(unsigned tty)
{
    if (tty >= BIOS->nvtty)
	return (-ENXIO);
    return (BIOS->vttys[tty].connected);
}

/*
 * Update the status of the rpc command buffer
 * Clean the cacheline afterwards
 */
BIOS_INLINE void
bios_write_rpc_status(bios_rpc_buffer *rpc, bios_rpc_status status)
{
    rpc->status = status;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
        BIOS_SCB_DCCMVAC = (intptr_t)&rpc->status;
        BIOS_DSB();
        BIOS_ISB();
    }
}

/*
 * Update the status of the rpc command buffer
 * Clean the cacheline afterwards
 */
BIOS_INLINE bios_rpc_status
bios_read_rpc_status(bios_rpc_buffer *rpc)
{
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	BIOS_SCB_DCIMVAC = (intptr_t)&rpc->status;
	BIOS_DSB();
	BIOS_ISB();
    }
    return (rpc->status);
}

/*
 * Update the status of the rpc command buffer
 * Clean the cacheline afterwards
 */
BIOS_INLINE void
bios_write_rpc_command(bios_rpc_buffer *rpc, bios_rpc_command command)
{
    rpc->command = command;
    BIOS_DSB();
    if (BIOS_DCACHE_ENABLED) {
        BIOS_SCB_DCCMVAC = (intptr_t)&rpc->command;
        BIOS_DSB();
        BIOS_ISB();
    }
}

/*
 * Update the status of the rpc command buffer
 * Clean the cacheline afterwards
 */
BIOS_INLINE bios_rpc_command
bios_read_rpc_command(bios_rpc_buffer *rpc)
{
    if (BIOS_DCACHE_ENABLED) {
	BIOS_DSB();
	BIOS_SCB_DCIMVAC = (intptr_t)&rpc->command;
	BIOS_DSB();
	BIOS_ISB();
    }
    return (rpc->command);
}

/*
 * Send a RPC command to the other core, and wait for a reply
 */
BIOS_INLINE int
bios_rpc_execute(bios_rpc_buffer *rpc, bios_rpc_command cmd, int nargs, const bios_rpc_arg *args)
{
    if (rpc->command != BIOS_CMD_NONE)
	return (-EBUSY);
    if (nargs < 0 || nargs > 7)
	return (-EINVAL);
    if (bios_read_rpc_status(rpc) != BIOS_RPC_IDLE)
	return (-EBUSY);
    memcpy(rpc->param, args, nargs * sizeof(bios_rpc_arg));
    bios_write_rpc_command(rpc, cmd);
    int rv, retry;
    for (retry = 0; retry < BIOS_COMMAND_RETRY; retry++) {
	bios_dmb_delay();
	bios_rpc_status status = bios_read_rpc_status(rpc);
	if (status < 0) {
	    rv = status;
	    goto _cleanup;
	}
	switch (status) {
	case BIOS_RPC_IDLE:
	case BIOS_RPC_RUNNING:
	    continue;
	case BIOS_RPC_SUCCESS:
	    rv = 0;
	    goto _cleanup;
	}
    }
    rv = -ETIMEDOUT;

_cleanup:
    /*
     * reset command and wait until status drop back to 0
     */
    bios_write_rpc_command(rpc, BIOS_CMD_NONE);
    for (retry = 0; retry < BIOS_WAITING_RETRY; retry++) {
	bios_dmb_delay();
	if (bios_read_rpc_status(rpc) == BIOS_RPC_IDLE)
	    break;
    }
    if (retry >= BIOS_WAITING_RETRY)
	rv = -ETIMEDOUT;
    return (rv);
}

#ifdef _BIOS_INLINE_DEFINED_
#undef BIOS_INLINE
#undef _BIOS_INLINE_DEFINED_
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BIOS_CORE_H_ */
