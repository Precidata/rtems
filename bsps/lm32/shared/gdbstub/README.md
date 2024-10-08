GDB Stub lm32
=============

This is a thread-aware gdb stub for the lm32 architecture. It has to be
linked with the application, which should be debugged. The application has
to call 'lm32_gdb_stub_install' to setup the stub.
   The stub remaps _all_ h/w exceptions to an own code (lm32-debug.S), which
saves all the registers, calls the gdb stub and restores the registers again.
   The interrupt exceptions gets handled in a special way. Because we remapped
this exception, we need to do
 - the same as the original one (in cpu_asm.S),
 - and, as we might use an ISR for breaking into a running application with
   gdb, we need to save all registers as well. To be backward compatible
   the missing callee saved registers gets appended to CPU_Interrupt_frame.
   There is a mapping in 'gdb_handle_break' for that.

To use this gdb stub, your bsp has to provide the following functions:
 - void gdb_put_debug_char(char c)
   Puts the given charater c to the debug console output. The function can
   block until the character can be written to the output buffer.

 - char gdb_get_debug_char(void)
   Returns the character in the input buffer of the debug console. If no one
   is availabe, the function must block.

 - void gdb_console_init()
   This function can be used to initialize the debug console. Additionally,
   it should set up the ISR for the debug console to call the function
   'gdb_handle_break', which is provided by the gdb stub and enable the
   interrupt for a break symbol on the debug serial port. If no ISR is
   provided, you won't be able to interrupt a running application.

 - void gdb_ack_irq()
   If an ISR is used, this function is used to acknowledge the interrupt.

NOTE: the stub don't skip a hardcoded 'break' in the code. So you have to
   set the PC an instruction further in the debugger (set $pc += 4).

NOTE2: make sure you have the following CFLAGS set:
     -mbarrel-shift-enabled -mmultiply-enabled -mdivide-enabled 
     -msign-extend-enabled
   Without the hardware support, it is done in software. Unfortunately, the
   stub also uses some shifts and multiplies. If you step through your code,
   there will be a chance that a breakpoint is set to one of that functions,
   which then causes an endless loop.


EXAMPLES

```shell
  char gdb_get_debug_char(void)
  {
    /* Wait until there is a byte in RXTX */
    while (!(uartread(LM32_UART_LSR) & LM32_UART_LSR_DR));

    return (char) uartread(LM32_UART_RBR);
  }

  void gdb_put_debug_char(char c)
  {
    /* Wait until RXTX is empty. */
    while (!(uartread(LM32_UART_LSR) & LM32_UART_LSR_THRE));
    uartwrite(LM32_UART_RBR, c);
  }

  extern void gdb_handle_break(
    rtems_vector_number vector,
    CPU_Interrupt_frame *frame
  );
  void gdb_console_init()
  {
    rtems_isr_entry old;

    /* enable interrupts */
    uartwrite(LM32_UART_IER, 1);

    rtems_interrupt_catch((rtems_isr_entry) gdb_handle_break, DEBUG_UART_IRQ,
      &old);
    lm32_interrupt_unmask(1 << DEBUG_UART_IRQ);
  }

  void gdb_ack_irq()
  {
    lm32_interrupt_ack(1 << DEBUG_UART_IRQ);
  }
```
