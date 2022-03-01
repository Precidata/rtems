/*
 * Copyright (c) 2018 embedded brains GmbH.  All rights reserved.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

/*
 * The content of this file was previously generated by the GPL licensed shgen
 * tool during the BSP build for a configured clock frequency
 * (CPU_CLOCK_RATE_HZ). All tools were removed from the RTEMS source repository
 * at some point in time. Tools with a BSD-style license were moved to the
 * RTEMS tools repository.
 */

#include <bsp.h>

int _sci_get_brparms(
  unsigned int   spd,
  unsigned char *smr,
  unsigned char *brr
)
{
  if (spd != 9600) {
    return -1;
  }

  *smr = 0x00;
  *brr = 0x40;
  return 0;
}
