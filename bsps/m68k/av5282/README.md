Avnet MCF5282 
=============
```
CPU: MCF5282, 59MHz
RAM: 16M
ROM: 8M
```
This is an evaluation board that uses the MCF5282 Coldfire CPU. It runs at about 59MHz scaled
from a 7.372MHz crystal and is integrated with the Avnet designed AvBus. 

ACKNOWLEDGEMENTS
----------------
This BSP is based on the work of:
    D. Peter Siddons
    Brett Swimley
    Jay Monkman
    Eric Norum
    Mike Bertosh

```
BSP NAME:           av5282
BOARD:              Avnet MCF5282
CPU FAMILY:         ColdFire 5282
CPU:                MCF5282
COPROCESSORS:       N/A

DEBUG MONITOR:      AVMON
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
PORT:               UART1 Terminal
ELECTRICAL:         RS-232
BAUD:               19200
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
```

Memory map
==========
      Memory map as set up by AVMON bootstrap and BSP initialization
```
          +--------------------------------------------------+
0000 0000 |    16 MByte SDRAM                                | 00FF FFFF
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
F000 0000 | 512 kByte on-chip flash (FLASHBAR)               |
          .                                                  .
FF80 0000 | External 8 MByte Flash memory                                                 .
          .                                                  .
          |                                                  | FFFF FFFF
          +--------------------------------------------------+
```


Interrupt map
=============
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

TIMING TESTS
------------
```
*** TIME TEST 1 ***
rtems_semaphore_create 28
rtems_semaphore_delete 31
rtems_semaphore_obtain: available 6
rtems_semaphore_obtain: not available -- NO_WAIT 7
rtems_semaphore_release: no waiting tasks 14
*** END OF TEST 1 ***

*** TIME TEST 2 ***
rtems_semaphore_obtain: not available -- caller blocks 57
*** END OF TEST 2 ***

*** TIME TEST 3 ***
rtems_semaphore_release: task readied -- preempts caller 39
*** END OF TEST 3 ***

*** TIME TEST 4 ***
rtems_task_restart: blocked task -- preempts caller 86
rtems_task_restart: ready task -- preempts caller 82
rtems_semaphore_release: task readied -- returns to caller 28
rtems_task_create 139
rtems_task_start 32
rtems_task_restart: suspended task -- returns to caller 42
rtems_task_delete: suspended task 99
rtems_task_restart: ready task -- returns to caller 44
rtems_task_restart: blocked task -- returns to caller 59
rtems_task_delete: blocked task 104
*** END OF TEST 4 ***

*** TIME TEST 5 ***
rtems_task_suspend: calling task 36
rtems_task_resume: task readied -- preempts caller 33
*** END OF TEST 5 ***

*** TIME TEST 6 ***
rtems_task_restart: calling task 45
rtems_task_suspend: returns to caller 12
rtems_task_resume: task readied -- returns to caller 15
rtems_task_delete: ready task 106
*** END OF TEST 6 ***

*** TIME TEST 7 ***
rtems_task_restart: suspended task -- preempts caller 68
*** END OF TEST 7 ***

*** TIME TEST 9 ***
rtems_message_queue_create 81
rtems_message_queue_send: no waiting tasks 30
rtems_message_queue_urgent: no waiting tasks 31
rtems_message_queue_receive: available 30
rtems_message_queue_flush: no messages flushed 12
rtems_message_queue_flush: messages flushed 18
rtems_message_queue_delete 42
*** END OF TEST 9 ***

*** TIME TEST 10 ***
rtems_message_queue_receive: not available -- NO_WAIT 16
rtems_message_queue_receive: not available -- caller blocks 58
*** END OF TEST 10 ***

*** TIME TEST 11 ***
rtems_message_queue_send: task readied -- preempts caller 53
*** END OF TEST 11 ***

*** TIME TEST 12 ***
rtems_message_queue_send: task readied -- returns to caller 35
*** END OF TEST 12 ***

*** TIME TEST 13 ***
rtems_message_queue_urgent: task readied -- preempts caller 51
*** END OF TEST 13 ***

*** TIME TEST 14 ***
rtems_message_queue_urgent: task readied -- returns to caller 33
*** END OF TEST 14 ***

*** TIME TEST 15 ***
rtems_event_receive: obtain current events 0
rtems_event_receive: not available -- NO_WAIT 9
rtems_event_receive: not available -- caller blocks 46
rtems_event_send: no task readied 7
rtems_event_receive: available 13
rtems_event_send: task readied -- returns to caller 19
*** END OF TEST 15 ***

*** TIME TEST 16 ***
rtems_event_send: task readied -- preempts caller 35
*** END OF TEST 16 ***

*** TIME TEST 17 ***
rtems_task_set_priority: preempts caller 56
*** END OF TEST 17 ***

*** TIME TEST 18 ***
rtems_task_delete: calling task 124
*** END OF TEST 18 ***

*** TIME TEST 19 ***
rtems_signal_catch 8
rtems_signal_send: returns to caller 17
rtems_signal_send: signal to self 29
exit ASR overhead: returns to calling task 23
exit ASR overhead: returns to preempting task 26
*** END OF TEST 19 ***

*** TIME TEST 20 ***
rtems_partition_create 29
rtems_region_create 59
rtems_partition_get_buffer: available 15
rtems_partition_get_buffer: not available 8
rtems_partition_return_buffer 16
rtems_partition_delete 14
rtems_region_get_segment: available 38
rtems_region_get_segment: not available -- NO_WAIT 41
rtems_region_return_segment: no waiting tasks 42
rtems_region_get_segment: not available -- caller blocks 80
rtems_region_return_segment: task readied -- preempts caller 108
rtems_region_return_segment: task readied -- returns to caller 86
rtems_region_delete 36
rtems_io_initialize 1
rtems_io_open 2
rtems_io_close 2
rtems_io_read 1
rtems_io_write 1
rtems_io_control 1
*** END OF TEST 20 ***

*** TIME TEST 21 ***
rtems_task_ident 73
rtems_message_queue_ident 74
rtems_semaphore_ident 85
rtems_partition_ident 73
rtems_region_ident 75
rtems_port_ident 73
rtems_timer_ident 76
rtems_rate_monotonic_ident 72
*** END OF TEST 21 *

*** TIME TEST 22 ***
rtems_message_queue_broadcast: task readied -- returns to caller 48
rtems_message_queue_broadcast: no waiting tasks 18
rtems_message_queue_broadcast: task readied -- preempts caller 58
*** END OF TEST 22 ***

*** TIME TEST 23 ***
rtems_timer_create 10
rtems_timer_fire_after: inactive 20
rtems_timer_fire_after: active 24
rtems_timer_cancel: active 8
rtems_timer_cancel: inactive 8
rtems_timer_reset: inactive 16
rtems_timer_reset: active 17
rtems_timer_fire_when: inactive 35
rtems_timer_fire_when: active 35
rtems_timer_delete: active 16
rtems_timer_delete: inactive 14
rtems_task_wake_when 53
*** END OF TEST 23 ***

*** TIME TEST 24 ***
rtems_task_wake_after: yield -- returns to caller 5
rtems_task_wake_after: yields -- preempts caller 30
*** END OF TEST 24 ***

*** TIME TEST 25 ***
rtems_clock_tick 11
*** END OF TEST 25 ***

*** TIME TEST 26 ***
_ISR_Disable 0
_ISR_Flash 0
_ISR_Enable 0
_Thread_Disable_dispatch 0
_Thread_Enable_dispatch 3
_Thread_Set_state 12
_Thread_Disptach (NO FP) 23
context switch: no floating point contexts 19
context switch: self 3
context switch: to another task 2
fp context switch: restore 1st FP task 19
fp context switch: save idle, restore initialized 4
fp context switch: save idle, restore idle 17
fp context switch: save initialized, restore initialized 4
_Thread_Resume 11
_Thread_Unblock 8
_Thread_Ready 7
_Thread_Get 4
_Semaphore_Get 2
_Thread_Get: invalid id 0
*** END OF TEST 26 ***

*** TIME TEST 27 ***
interrupt entry overhead: returns to interrupted task 5
interrupt exit overhead: returns to interrupted task 4
interrupt entry overhead: returns to nested interrupt 3
interrupt exit overhead: returns to nested interrupt 3
interrupt entry overhead: returns to preempting task 6
interrupt exit overhead: returns to preempting task 30
*** END OF TEST 27 ***

*** TIME TEST 28 ***
rtems_port_create 18
rtems_port_external_to_internal 6
rtems_port_internal_to_external 7
rtems_port_delete 18
*** END OF TEST 28 ***

*** TIME TEST 29 ***
rtems_rate_monotonic_create 18
rtems_rate_monotonic_period: initiate period -- returns to caller 29
rtems_rate_monotonic_period: obtain status 15
rtems_rate_monotonic_cancel 19
rtems_rate_monotonic_delete: inactive 22
rtems_rate_monotonic_delete: active 24
rtems_rate_monotonic_period: conclude periods -- caller blocks 36
*** END OF TEST 29 ***

*** TIME CHECKER ***
Units may not be in microseconds for this test!!!
0 100000
Total time = 0
Average time = 0
<pause>
NULL timer stopped at 0
LOOP (1000) timer stopped at 225
LOOP (10000) timer stopped at 2242
LOOP (50000) timer stopped at 11207
LOOP (100000) timer stopped at 22414
*** END OF TIME CHECKER ***

*** TIME TEST OVERHEAD ***
rtems_initialize_executive 0
rtems_shutdown_executive 0
rtems_task_create 1
rtems_task_ident 0
rtems_task_start 0
rtems_task_restart 0
rtems_task_delete 0
rtems_task_suspend 0
rtems_task_resume 0
rtems_task_set_priority 0
rtems_task_mode 0
rtems_task_wake_when 1
rtems_task_wake_after 0
rtems_interrupt_catch 0
rtems_clock_get 1
rtems_clock_set 1
rtems_clock_tick 0
<pause>
rtems_timer_create 0
rtems_timer_delete 0
rtems_timer_ident 0
rtems_timer_fire_after 0
rtems_timer_fire_when 1
rtems_timer_reset 0
rtems_timer_cancel 0
rtems_semaphore_create 1
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
rtems_partition_create 1
rtems_partition_ident 0
rtems_partition_delete 0
rtems_partition_get_buffer 0
rtems_partition_return_buffer 0
rtems_region_create 1
rtems_region_ident 0
rtems_region_delete 0
rtems_region_get_segment 1
rtems_region_return_segment 0
rtems_port_create 1
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
