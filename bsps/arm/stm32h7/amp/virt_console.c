
#include <bsp.h>
#include <bsp/irq.h>
#include <bsp/fatal.h>
#include <rtems/console.h>

#include <rtems/termiostypes.h>
#include <rtems/fatal.h>

#include <rtems/bspIo.h>
#include <rtems/sysinit.h>
#include <stm32h7/memory.h>

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <errno.h>

#include <bios_core.h>

#ifdef STM32H7_SLAVE_BSP

#define MAX_NUMBER_OF_VTTY 10

static void
virt_console_putchar(char c)
{
    for (;;) {
	//  int rv = bios_console_write(&c, 1);
	int rv = bios_vtty_write(0, &c, 1);
	switch (rv) {
	case -EAGAIN:
	    continue;
	default:
	    return;
	}
    }
}

typedef struct {
  rtems_termios_device_context device;
  uint32_t vtty_no;
} virt_uart_context;

virt_uart_context virt_uart_instances[MAX_NUMBER_OF_VTTY];

static inline virt_uart_context *virt_uart_get_context(
  rtems_termios_device_context *base
)
{
    size_t i;
    for (i = 0; i < (BIOS->nvtty < MAX_NUMBER_OF_VTTY ? BIOS->nvtty : MAX_NUMBER_OF_VTTY); i++) {
	if (&(virt_uart_instances[i].device) == base)
	    return &virt_uart_instances[i];
    }
    return NULL;
}

static void virt_tty_write(
  rtems_termios_device_context *base,
  const char *buf,
  size_t len
)
{
    virt_uart_context* ctx = virt_uart_get_context(base);
    rtems_interrupt_level level;

    while (len > 0) {
	rtems_interrupt_disable(level);
	int rv = bios_vtty_write(ctx->vtty_no, buf, len);
	rtems_interrupt_enable(level);
	if (rv == (-EAGAIN))
	    continue;
	len -= rv;
	buf += rv;
    }
}

static int virt_tty_read(rtems_termios_device_context *base)
{
    virt_uart_context* ctx = virt_uart_get_context(base);
    rtems_interrupt_level level;

    /* need to initialize character holder to 0 and not to -1
     * since if we do -1, then assigning to it one character blow
     * would still result in c < 0 which would break termios processing
     */
    int c = 0;

    for (;;) {
	rtems_interrupt_disable(level);
	int rv = bios_vtty_read(ctx->vtty_no, &c, 1);
	rtems_interrupt_enable(level);
	if (rv == (-EAGAIN))
	    continue;
	return c;
    }
    /* unreachable code */
    return c;
}

static void virt_output_char_init(void)
{
  BSP_output_char = virt_console_putchar;
}

static const rtems_termios_device_handler virt_uart_handler = {
  .first_open = NULL,
  .last_close = NULL,
  .write = virt_tty_write,
  .set_attributes = NULL,
  .poll_read = virt_tty_read,
  .mode = TERMIOS_POLLED
};

rtems_status_code console_initialize(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void *arg
)
{
  size_t i;

  rtems_termios_initialize();

  for (i = 0; i < (BIOS->nvtty < MAX_NUMBER_OF_VTTY ? BIOS->nvtty : MAX_NUMBER_OF_VTTY); i++) {
    virt_uart_context *ctx;
    char path[sizeof("/dev/ttySXXX")];

    ctx = &virt_uart_instances[i];
    ctx->vtty_no = i;

    snprintf(path, sizeof(path), "/dev/ttyS%" PRIu8, ctx->vtty_no);
    rtems_termios_device_install(
      path,
      &virt_uart_handler,
      NULL,
      &ctx->device
    );
  }
  link("/dev/ttyS0", CONSOLE_DEVICE_NAME);
  return RTEMS_SUCCESSFUL;
}

BSP_output_char_function_type BSP_output_char = virt_console_putchar;

BSP_polling_getchar_function_type BSP_poll_char = NULL;

RTEMS_SYSINIT_ITEM(
  virt_output_char_init,
  RTEMS_SYSINIT_BSP_START,
  RTEMS_SYSINIT_ORDER_LAST_BUT_5
);

#endif


