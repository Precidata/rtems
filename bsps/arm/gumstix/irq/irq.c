/*
 * Copyright (c) 2010 embedded brains GmbH & Co. KG
 *
 * PXA255 Interrupt handler by Yang Xi <hiyangxi@gmail.com>
 * Copyright (c) 2004 by Jay Monkman <jtm@lopingdog.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/score/armv4.h>

#include <bsp.h>
#include <bsp/irq.h>
#include <bsp/irq-generic.h>

#include <pxa255.h>

void bsp_interrupt_dispatch(void)
{
  rtems_vector_number vector = 31 - __builtin_clz(XSCALE_INT_ICIP);

  bsp_interrupt_handler_dispatch(vector);
}

rtems_status_code bsp_interrupt_get_attributes(
  rtems_vector_number         vector,
  rtems_interrupt_attributes *attributes
)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_is_pending(
  rtems_vector_number vector,
  bool               *pending
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(pending != NULL);
  *pending = false;
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_raise(rtems_vector_number vector)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_clear(rtems_vector_number vector)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_vector_is_enabled(
  rtems_vector_number vector,
  bool               *enabled
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(enabled != NULL);
  *enabled = false;
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  XSCALE_INT_ICMR |= 1 << vector;
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  XSCALE_INT_ICMR  &= ~(1 << vector);
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_set_priority(
  rtems_vector_number vector,
  uint32_t priority
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_get_priority(
  rtems_vector_number vector,
  uint32_t *priority
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(priority != NULL);
  return RTEMS_UNSATISFIED;
}

void bsp_interrupt_facility_initialize(void)
{
  /* disable all interrupts */
  XSCALE_INT_ICMR = 0x0;

  /* Direct the interrupt to IRQ*/
  XSCALE_INT_ICLR = 0x0;

  /* Install the IRQ exception handler */
  _CPU_ISR_install_vector(ARM_EXCEPTION_IRQ, _ARMV4_Exception_interrupt, NULL);
}
