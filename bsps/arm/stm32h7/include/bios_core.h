/*
 * $Id: bios_core.h 3256 2023-03-09 07:29:10Z cedric $
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
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIOS_MAGIC_V1		0xfacade01

#ifndef BIOS_INLINE
#define BIOS_INLINE static inline __attribute__((always_inline))
#define _BIOS_INLINE_DEFINED_
#endif

/*
 * BIOS root structure is located at beginning of SRAM4
 */
#define BIOS		((struct bios_root *)0x38000000)




/*
 * LED colors
 */
typedef enum bios_led_color {
	BIOS_LED_BLACK	 = 0x0000,
	BIOS_LED_RED	 = 0x0001,
	BIOS_LED_GREEN	 = 0x0002,
	BIOS_LED_YELLOW  = 0x0003,
	BIOS_LED_BLUE	 = 0x0004,
	BIOS_LED_MAGENTA = 0x0005,
	BIOS_LED_CYAN	 = 0x0006,
	BIOS_LED_WHITE	 = 0x0007,
	BIOS_LED_MAX,
} bios_led_color;

typedef enum bios_core_state {
	/* for use by BIOS only */
	BIOS_CORE_INIT = 0,		/* initial state, core not working */
	BIOS_CORE_WAITING = 1,		/* synchronizing with the other core */

	/* for use by BIOS and app */
	BIOS_CORE_STARTING = 2,		/* running initialization code */
	BIOS_CORE_RUNNING = 3,		/* running application code */
	BIOS_CORE_STOPPING = 4,		/* shutting down */
	BIOS_CORE_STOPPED = 5,		/* initial state, core not working */
	BIOS_CORE_CRASHED = 6,		/* crashed for some reason */
} bios_core_state;


/********** structures **********/

/*
 * Version number of the software. Little endian.
 * build:
 *	0x00-0x3f: alpha versions
 *	0x40-0x7f: beta versions
 *	0x80-0xbf: release candidates
 *	0xc0-0xff: released versions
 */
typedef union bios_version {
	uint32_t		 u32;
	struct {
		uint8_t		 build;
		uint8_t		 micro;
		uint8_t		 minor;
		uint8_t		 major;
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
	volatile uint32_t	 magic;
	volatile bios_core_state state;
	volatile uint32_t	 updates;
	bios_version		 version;
} bios_core_info;

/*
 * Unidirectional ring buffer for byte elements
 */
typedef struct bios_byte_queue {
	uint8_t			*buf;		/* ring buffer */
	uint32_t		 size;		/* number of bytes, power of 2 */
	volatile uint32_t	 head;		/* read pointer */
	volatile uint32_t	 tail;		/* write pointer */
} bios_byte_queue;

/*
 * Virtual TTY, to connect to a shell running on the CM7
 */
typedef struct bios_virtual_tty {
	struct bios_byte_queue	*inq;		/* CM4 => CM7 */
	struct bios_byte_queue	*outq;		/* CM7 => CM4 */
	bool			 connected;	/* CM4 => CM7 */
} bios_virtual_tty;

/*
 * Root BIOS structure, located at the beginning of SRAM4
 */
typedef struct bios_root {
	bios_core_info		cm4;		/* CM4 => CM7 */
	bios_core_info		cm7;		/* CM7 => CM4 */
	uint8_t                 iled[4];	/* CM7 => CM4 */
	bios_byte_queue		*console;	/* initialized by CM4 */
	uint32_t		 nvtty;		/* number of virtual ttys */
	bios_virtual_tty	*vttys;		/* list of virtual ttys */
} bios_root;

/********** functions **********/

BIOS_INLINE void
bios_inst_led_blink(enum bios_led_color c1, enum bios_led_color c2,
	enum bios_led_color c3, enum bios_led_color c4)
{
	BIOS->iled[0] = (uint8_t)c1;
	BIOS->iled[1] = (uint8_t)c2;
	BIOS->iled[2] = (uint8_t)c3;
	BIOS->iled[3] = (uint8_t)c4;
}

BIOS_INLINE void
bios_inst_led_solid(enum bios_led_color c)
{
	bios_inst_led_blink(c, c, c, c);
}

BIOS_INLINE int
bios_bqe_write(bios_byte_queue *bq, const void *buf, unsigned n)
{
	uint32_t size = bq->size;
	uint32_t head = bq->head;
	uint32_t tail = bq->tail;
	uint32_t used = tail - head;

	if (n > size)
		return (-EINVAL);	/* message too big */
	if (used > size)
		return (-EFAULT);	/* corrupted queue */
	if (used == size)
		return (-EAGAIN);	/* queue full */
	if (n > size - used)
		n = size - used;	/* partial write */
	uint32_t i = tail;
	uint32_t mask = size - 1;
	const uint8_t *p = (const uint8_t *)buf;
	for (unsigned j = 0; j < n; j++)
		bq->buf[i++ & mask] = *p++;
	asm volatile ("dmb" : : : "memory");
	bq->tail = i;
        asm volatile ("dmb" : : : "memory");
	return (n);
}

BIOS_INLINE int
bios_bqe_read(bios_byte_queue *bq, void *buf, unsigned n)
{
	uint32_t size = bq->size;
	uint32_t head = bq->head;
	uint32_t tail = bq->tail;
	uint32_t used = tail - head;

	if (used > size)
		return (-EFAULT);	/* corrupted queue */
	if (used == 0)
		return (-EAGAIN);	/* queue empty */
	if (n > used)
		n = used;		/* partial read */
	uint32_t i = head;
	uint32_t mask = size - 1;
	uint8_t *p = (uint8_t *)buf;
	for (unsigned j = 0; j < n; j++)
		*p++ = bq->buf[i++ & mask];
	bq->head = i;
	return (n);
}

BIOS_INLINE int
bios_console_write(const void *buf, unsigned n)
{
	return (bios_bqe_write(BIOS->console, buf, n));
}

BIOS_INLINE int
bios_vtty_write(unsigned tty, const void *buf, unsigned n)
{
	if (tty >= BIOS->nvtty)
		return (-ENXIO);
	return (bios_bqe_write(BIOS->vttys[tty].outq, buf, n));
}

BIOS_INLINE int
bios_vtty_read(unsigned tty, void *buf, unsigned n)
{
	if (tty >= BIOS->nvtty)
		return (-ENXIO);
	return (bios_bqe_read(BIOS->vttys[tty].inq, buf, n));
}

BIOS_INLINE int
bios_vtty_is_connected(unsigned tty)
{
	if (tty >= BIOS->nvtty)
		return (-ENXIO);
	return (BIOS->vttys[tty].connected);
}

#ifdef _BIOS_INLINE_DEFINED_
#undef BIOS_INLINE
#undef _BIOS_INLINE_DEFINED_
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BIOS_CORE_H_ */
