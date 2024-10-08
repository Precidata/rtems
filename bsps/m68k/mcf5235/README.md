Motorola MCF5235EVB
===================
CPU: MCF5235, 150MHz
RAM: 16M
ROM: 2M

This is a Motorola evaluation board that uses the MCF5235 Coldfire CPU. 
This board is running at 150MHz scaled from a 25MHz oscillator.

By default the BSP creates an image file for use when loaded into the
RAM of the evaluation board.  To create an image file to boot from flash
add the following command to the applications Makefile:
LDFLAGS += -qnolinkcmds -T linkcmdsflash

Note: This BSP has also been tested with the Freescale / Axiom Manufacturing
(M5235BCC Business Card Controller) evaluation board.


ACKNOWLEDGEMENTS:
-----------------
This BSP is heavily based on the work of:
    D. Peter Siddons
    Brett Swimley
    Jay Monkman
    Eric Norum
    Mike Bertosh

```     
BSP NAME:           mcf5235
BOARD:              Motorola MCF5235EVB
CPU FAMILY:         ColdFire 5235
CPU:                MCF5235
COPROCESSORS:       N/A

DEBUG MONITOR:      dBUG
```

PERIPHERALS
-----------
```
TIMERS:             Four PIT (RTEMS clock is PIT3), Four Timers
RESOLUTION:         10 microsecond 
SERIAL PORTS:       Internal UART 1, 2 and 3
REAL-TIME CLOCK:    none
DMA:                none
VIDEO:              none
SCSI:               none
NETWORKING:         Internal 10/100MHz FEC
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       PIT3
IOSUPP DRIVER:      none
SHMSUPP:            none
TIMER DRIVER:       TIMER3
TTY DRIVER:         UART1, 2 and 3
```

STDIO
-----
```
PORT:               UART0 Terminal
ELECTRICAL:         RS-232
BAUD:               19200
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
```

Memory map
----------
      Memory map as set up by dBUG bootstrap and BSP initialization

```
          +--------------------------------------------------+
0000 0000 |       16 MByte SDRAM                             | 00FF FFFF
0100 0000 |  ---------------------------------------------   |
          |    Address space for future SDRAM expansion      |
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | 0FFF FFFF
          +--------------------------------------------------+
1000 0000 |                                                  |
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | 1FFF FFFF
          +--------------------------------------------------+
2000 0000 | 64 kByte on-chip SRAM (RAMBAR)                   |
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | 2FFF FFFF
          +--------------------------------------------------+
3000 0000 |                                                  | 30FF FFFF
          .                                                  .
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | 3FFF FFFF
          +--------------------------------------------------+
4000 0000 | Internal peripheral system (IPSBAR)              |
          .                                                  .
          |                                                  |
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | 4FFF FFFF
          +--------------------------------------------------+
          .                                                  .
          .                                                  .
          .                                                  .
          +--------------------------------------------------+
FFE0 0000 | External 4 MByte Flash                           |
          .                                                  .
          .                                                  .
          .                                                  .
          |                                                  | FFFF FFFF
          +--------------------------------------------------+
```

Interrupt map
-------------
```
+-----+-----------------------------------------------------------------------+
|     |                                PRIORITY                               |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|LEVEL|    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  7  |        |        |        |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  6  |        |        |        |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  5  |        |        |        |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  4  | FEC RX | FEC TX |        |        |        |        |        |   PIT  |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  3  | UART 0 | UART 1 | UART 2 |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  2  |        |        |        |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
|  1  |        |        |        |        |        |        |        |        |
+-----+--------+--------+--------+--------+--------+--------+--------+--------+
```

Timing tests
------------
```
************************************
*** TIME TEST 1 ***
rtems_semaphore_create 12
rtems_semaphore_delete 11
rtems_semaphore_obtain: available 2
rtems_semaphore_obtain: not available -- NO_WAIT 3
rtems_semaphore_release: no waiting tasks 6
*** END OF TEST 1 ***

*** TIME TEST 2 ***
rtems_semaphore_obtain: not available -- caller blocks 18
*** END OF TEST 2 ***

*** TIME TEST 3 ***
rtems_semaphore_release: task readied -- preempts caller 12
*** END OF TEST 3 ***

*** TIME TEST 4 ***
rtems_task_restart: blocked task -- preempts caller 31
rtems_task_restart: ready task -- preempts caller 30
rtems_semaphore_release: task readied -- returns to caller 8
rtems_task_create 45
rtems_task_start 9
rtems_task_restart: suspended task -- returns to caller 14
rtems_task_delete: suspended task 32
rtems_task_restart: ready task -- returns to caller 14
rtems_task_restart: blocked task -- returns to caller 21
rtems_task_delete: blocked task 32
*** END OF TEST 4 ***

*** TIME TEST 5 ***
rtems_task_suspend: calling task 15
rtems_task_resume: task readied -- preempts caller 9
*** END OF TEST 5 ***

*** TIME TEST 6 ***
rtems_task_restart: calling task 12
rtems_task_suspend: returns to caller 5
rtems_task_resume: task readied -- returns to caller 6
rtems_task_delete: ready task 34
*** END OF TEST 6 ***

*** TIME TEST 7 ***
rtems_task_restart: suspended task -- preempts caller 22
*** END OF TEST 7 ***

*** TIME TEST 9 ***
rtems_message_queue_create 37
rtems_message_queue_send: no waiting tasks 11
rtems_message_queue_urgent: no waiting tasks 10
rtems_message_queue_receive: available 10
rtems_message_queue_flush: no messages flushed 3
rtems_message_queue_flush: messages flushed 5
rtems_message_queue_delete 17
*** END OF TEST 9 ***

*** TIME TEST 10 ***
rtems_message_queue_receive: not available -- NO_WAIT 6
rtems_message_queue_receive: not available -- caller blocks 20
*** END OF TEST 10 ***

*** TIME TEST 11 ***
rtems_message_queue_send: task readied -- preempts caller 17
*** END OF TEST 11 ***

*** TIME TEST 12 ***
rtems_message_queue_send: task readied -- returns to caller 12
*** END OF TEST 12 ***

*** TIME TEST 13 ***
rtems_message_queue_urgent: task readied -- preempts caller 20
*** END OF TEST 13 ***

*** TIME TEST 14 ***
rtems_message_queue_urgent: task readied -- returns to caller 14
*** END OF TEST 14 ***

*** TIME TEST 15 ***
rtems_event_receive: obtain current events 0
rtems_event_receive: not available -- NO_WAIT 3
rtems_event_receive: not available -- caller blocks 18
rtems_event_send: no task readied 3
rtems_event_receive: available 5
rtems_event_send: task readied -- returns to caller 7
*** END OF TEST 15 ***

*** TIME TEST 16 ***
rtems_event_send: task readied -- preempts caller 12
*** END OF TEST 16 ***

*** TIME TEST 17 ***
rtems_task_set_priority: preempts caller 21
*** END OF TEST 17 ***

*** TIME TEST 18 ***
rtems_task_delete: calling task 40
*** END OF TEST 18 ***

*** TIME TEST 19 ***
rtems_signal_catch 3
rtems_signal_send: returns to caller 6
rtems_signal_send: signal to self 11
exit ASR overhead: returns to calling task 8
exit ASR overhead: returns to preempting task 10
*** END OF TEST 19 ***

*** TIME TEST 20 ***
rtems_partition_create 13
rtems_region_create 24
rtems_partition_get_buffer: available 6
rtems_partition_get_buffer: not available 4
rtems_partition_return_buffer 6
rtems_partition_delete 6
rtems_region_get_segment: available 12
rtems_region_get_segment: not available -- NO_WAIT 13
rtems_region_return_segment: no waiting tasks 12
rtems_region_get_segment: not available -- caller blocks 30
rtems_region_return_segment: task readied -- preempts caller 40
rtems_region_return_segment: task readied -- returns to caller 25
rtems_region_delete 12
rtems_io_initialize 0
rtems_io_open 0
rtems_io_close 0
rtems_io_read 0
rtems_io_write 0
rtems_io_control 0
*** END OF TEST 20 ***

*** TIME TEST 21 ***
rtems_task_ident 31
rtems_message_queue_ident 30
rtems_semaphore_ident 34
rtems_partition_ident 30
rtems_region_ident 30
rtems_port_ident 29
rtems_timer_ident 30
rtems_rate_monotonic_ident 30
*** END OF TEST 21 ***

*** TIME TEST 22 ***
rtems_message_queue_broadcast: task readied -- returns to caller 19
rtems_message_queue_broadcast: no waiting tasks 6
rtems_message_queue_broadcast: task readied -- preempts caller 20
*** END OF TEST 22 ***

*** TIME TEST 23 ***
rtems_timer_create 4
rtems_timer_fire_after: inactive 6
rtems_timer_fire_after: active 6
rtems_timer_cancel: active 4
rtems_timer_cancel: inactive 3
rtems_timer_reset: inactive 6
rtems_timer_reset: active 6
rtems_timer_fire_when: inactive 8
rtems_timer_fire_when: active 8
rtems_timer_delete: active 5
rtems_timer_delete: inactive 5
rtems_task_wake_when 16
*** END OF TEST 23 ***

*** TIME TEST 24 ***
rtems_task_wake_after: yield -- returns to caller 2
rtems_task_wake_after: yields -- preempts caller 12
*** END OF TEST 24 ***

*** TIME TEST 25 ***
rtems_clock_tick 4
*** END OF TEST 25 ***

*** TIME TEST 26 ***
_ISR_Disable 0
_ISR_Flash 0
_ISR_Enable 0
_Thread_Disable_dispatch 0
_Thread_Enable_dispatch 1
_Thread_Set_state 4
_Thread_Disptach (NO FP) 9
context switch: no floating point contexts 7
context switch: self 1
context switch: to another task 1
fp context switch: restore 1st FP task 6
fp context switch: save idle, restore initialized 2
fp context switch: save idle, restore idle 6
fp context switch: save initialized, restore initialized 1
_Thread_Resume 4
_Thread_Unblock 3
_Thread_Ready 2
_Thread_Get 0
_Semaphore_Get 0
_Thread_Get: invalid id 0
*** END OF TEST 26 ***

*** TIME TEST 27 ***
interrupt entry overhead: returns to interrupted task 2
interrupt exit overhead: returns to interrupted task 1
interrupt entry overhead: returns to nested interrupt 1
interrupt exit overhead: returns to nested interrupt 1
interrupt entry overhead: returns to preempting task 2
interrupt exit overhead: returns to preempting task 12
*** END OF TEST 27 ***

*** TIME TEST 28 ***
rtems_port_create 8
rtems_port_external_to_internal 2
rtems_port_internal_to_external 3
rtems_port_delete 7
*** END OF TEST 28 ***

*** TIME TEST 29 ***
rtems_rate_monotonic_create 8
rtems_rate_monotonic_period: initiate period -- returns to caller 12
rtems_rate_monotonic_period: obtain status 5
rtems_rate_monotonic_cancel 7
rtems_rate_monotonic_delete: inactive 8
rtems_rate_monotonic_delete: active 7
rtems_rate_monotonic_period: conclude periods -- caller blocks 11
*** END OF TEST 29 ***

*** TIME CHECKER ***
Units may not be in microseconds for this test!!!
0 100000
Total time = 0
Average time = 0
<pause>
NULL timer stopped at 0
LOOP (1000) timer stopped at 94
LOOP (10000) timer stopped at 941
LOOP (50000) timer stopped at 4704
LOOP (100000) timer stopped at 9408
*** END OF TIME CHECKER ***

*** TIME TEST OVERHEAD ***
rtems_initialize_executive 0
rtems_shutdown_executive 0
rtems_task_create 0
rtems_task_ident 0
rtems_task_start 0
rtems_task_restart 0
rtems_task_delete 0
rtems_task_suspend 0
rtems_task_resume 0
rtems_task_set_priority 0
rtems_task_mode 0
rtems_task_wake_when 0
rtems_task_wake_after 0
rtems_interrupt_catch 0
rtems_clock_get 0
rtems_clock_set 0
rtems_clock_tick 0
<pause>
rtems_timer_create 0
rtems_timer_delete 0
rtems_timer_ident 0
rtems_timer_fire_after 0
rtems_timer_fire_when 0
rtems_timer_reset 0
rtems_timer_cancel 0
rtems_semaphore_create 0
rtems_semaphore_delete 0
rtems_semaphore_ident 0
rtems_semaphore_obtain 0
rtems_semaphore_release 0
rtems_message_queue_create 0
rtems_message_queue_ident 0
rtems_message_queue_delete 0
rtems_message_queue_send 0
rtems_message_queue_urgent 0
rtems_message_queue_broadcast 0
rtems_message_queue_receive 0
rtems_message_queue_flush 0
<pause>
rtems_event_send 0
rtems_event_receive 0
rtems_signal_catch 0
rtems_signal_send 0
rtems_partition_create 0
rtems_partition_ident 0
rtems_partition_delete 0
rtems_partition_get_buffer 0
rtems_partition_return_buffer 0
rtems_region_create 0
rtems_region_ident 0
rtems_region_delete 0
rtems_region_get_segment 0
rtems_region_return_segment 0
rtems_port_create 0
rtems_port_ident 0
rtems_port_delete 0
rtems_port_external_to_internal 0
rtems_port_internal_to_external 0
<pause>
rtems_io_initialize 0
rtems_io_open 0
rtems_io_close 0
rtems_io_read 0
rtems_io_write 0
rtems_io_control 0
rtems_fatal_error_occurred 0
rtems_rate_monotonic_create 0
rtems_rate_monotonic_ident 0
rtems_rate_monotonic_delete 0
rtems_rate_monotonic_cancel 0
rtems_rate_monotonic_period 0
rtems_multiprocessing_announce 0
*** END OF TIME OVERHEAD ***
```
