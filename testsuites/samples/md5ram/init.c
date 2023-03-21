/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <tmacros.h>

#include <md5.h>

#include <stm32h7/memory.h>

const char rtems_test_name[] = "HELLO WORLD";

uint32_t HAL_GetTick(void);

//#define HAL_GetTick() _Watchdog_Ticks_since_boot

static rtems_task Init(
  rtems_task_argument ignored
)
{
//    void* base = (void*)0x70000000;
  //void* base = (void*)stm32h7_memory_sram_axi_begin;
  //uint32_t sz = (stm32h7_memory_sram_axi_end - stm32h7_memory_sram_axi_begin) / 1024;
  void* base = (void*)stm32h7_memory_sdram_1_begin;
  uint32_t sz = (stm32h7_memory_sdram_1_end - stm32h7_memory_sdram_1_begin) / 1024;
  rtems_print_printer_fprintf_putc(&rtems_test_printer);
  TEST_BEGIN();
  printf( "MD5 RAM Tester.\n" );
  printf( "SystemCoreClock: %d\n", stm32h7_systick_frequency());
  printf( "Testing RAM starting @ %p with size: %x\n", base, sz * 1024);
  MD5_CTX ctx;
  unsigned char digest[16];
  unsigned char prev[16];
  int pass = 0;
  uint32_t pass_start;
  uint32_t pass_end;
  uint32_t test_start;
  uint32_t test_end;

  test_start = HAL_GetTick();
  for (size_t k = 0; k < 1000000; k++) {
      MD5Init(&ctx);
      pass_start = HAL_GetTick();
      for (size_t i = 0; i < sz; i++) {
          MD5Update(&ctx, base + i * 1024, 1024);
      }
      pass_end = HAL_GetTick();
      MD5Final(digest, &ctx);
      printf("\r%d loop, %d. pass, %d ms, md5: ", k, (pass + 1), (pass_end - pass_start));
      for (int i = 0; i < 16; i++) {
          printf("%02x", digest[i]);
      }
      if (pass == 0) {
          // let's copy digest to prev and not compare, this is our first pass
          memcpy(prev, (const char*)digest, 16);
          pass++;
      }
      else {
          if (memcmp(prev, digest, 16) != 0) {
              printf(" *ERROR* old MD5: ");
              for (int i = 0; i < 16; i++) {
                  printf("%02x", prev[i]);
              }
              printf("\n");
              memcpy(prev, (const char*)digest, 16);
              pass = 0;
          }
          else {
              pass++;
          }
      }
  }
  test_end = HAL_GetTick();
  printf("\r\nTest run for %d seconds.", ((test_end - test_start) / 1000));
  TEST_END();
  rtems_test_exit( 0 );
}


/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
