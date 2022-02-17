/**
 * @file
 *
 * @ingroup libblock
 *
 * @brief Provide SRAM support for the NV Disk
 */

/*
 * Copyright 2007 Chris Johns (chrisj@rtems.org)
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <rtems.h>

#include <rtems/nvdisk-sram.h>

#ifndef NVDISK_SRAM_ERROR_TRACE
#define NVDISK_SRAM_ERROR_TRACE (0)
#endif

static int
rtems_nvdisk_sram_read (uint32_t device RTEMS_UNUSED,
                        uint32_t flags RTEMS_UNUSED,
                        void*    base,
                        uint32_t offset,
                        void*    buffer,
                        size_t   size)
{
  memcpy (buffer, (base + offset), size);
  return 0;
}

static int
rtems_nvdisk_sram_write (uint32_t    device RTEMS_UNUSED,
                         uint32_t    flags RTEMS_UNUSED,
                         void*       base,
                         uint32_t    offset,
                         const void* buffer,
                         size_t      size)
{
  memcpy ((base + offset), buffer, size);
  return 0;
}

static int
rtems_nvdisk_sram_verify (uint32_t    device RTEMS_UNUSED,
                          uint32_t    flags RTEMS_UNUSED,
                          void*       base,
                          uint32_t    offset,
                          const void* buffer,
                          size_t      size)
{
  return memcmp ((base + offset), buffer, size) == 0 ? 0 : EIO;
}


const rtems_nvdisk_driver_handlers rtems_nvdisk_sram_handlers =
{
  .read   = rtems_nvdisk_sram_read,
  .write  = rtems_nvdisk_sram_write,
  .verify = rtems_nvdisk_sram_verify
};
