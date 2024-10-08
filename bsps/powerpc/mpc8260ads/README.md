mpc8260ads
==========
```
BSP NAME:           mpc8260ads
BOARD:              Motorola MPC8260 ADS Evaluation board
BUS:                N/A
CPU FAMILY:         ppc
CPU:                PowerPC MPC8260
COPROCESSORS:       Hardware FPU (except on revision 2J24M)
MODE:               32 bit mode, I and D cache disabled
DEBUG MONITOR:      None
```

PERIPHERALS
-----------
```
TIMERS:             Decrementer
RESOLUTION:         0.1 microsecond
SERIAL PORTS:       4 SCCs (SSC1 and 2 are connectd to RS232 drivers)
                    SCC3 is used in HDLC mode to transport IP frames.
                    SMCs, FCCs, SPI, I2C are unused.
REAL-TIME CLOCK:    
DMA:                Each serial port
VIDEO:              none
SCSI:               none
NETWORKING:         IP over HDLC (8 Mbps) on SCC3 (MPC8260)
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       Decrementer
IOSUPP DRIVER:      SCC1, SCC2
SHMSUPP:            none
TIMER DRIVER:       Timebase register (lower 32 bits only)
```

STDIO
-----
```
PORT:               SCC2
ELECTRICAL:         RS-232
BAUD:               9600
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
```

NOTES
-----
```
On-chip resources:
	SCC1		console
	SCC2		console
	SCC3		network
	SCC4		
	CLK1		
	CLK2		
	CLK3
	CLK4
	CLK5		network
	CLK6
	CLK7
	CLK8
	BRG1		console
	BRG2		console
	BRG3		console
	BRG4		network
	RTC
	PIT		
	TB		timer
	DEC		clock
	SWT	
	*CS0		8M FLASH
	*CS1		Config registers
	*CS2		60X SDRAM
	*CS3		
	*CS4		LCL SDRAM
	*CS5		ATM
	*CS6		
	*CS7		
	*CS8
	*CS9
	*CS10
	*CS11				
	UPMA
	UPMB
	IRQ0		
	IRQ1		
	IRQ2		
	IRQ3		
	IRQ4		
	IRQ5
	IRQ6	
	IRQ7
```

Board description
-----------------
Clock rate:	40MHz (board can run up 66MHz with alternate OSC)
Bus width:	32 bit Flash, 64 bit SDRAM
FLASH:		8M SIMM
RAM:		16M SDRAM DIMM

The board is marked with "Rev PILOT"
U17 is marked with "MPC8260ADS Pilot 00"
The processor is marked with "XPC8260ZU166 166/133/66 MHz"


Board Configuration:
--------------------
The evaluation board has a number of configurable options:

DIP switch settings used:
DS1: 1-"off", 2-"on", 3-"off", 4-"on", 5-"off", 6-"off", 7-"off", 8-"off"
DS2: all "on"
DS3: all "on"

A 40MHz oscillator is fitted to U16.


Board Connections:
------------------
Connect a serial terminal to PA3 (SCC2) configured for 9600,n,8,1 to
get console I/O. A 9way male-female straight-through cable is required to 
connect to a PC.

If you require the network connections (see README in network directory)
you need to connect 3VTTL - RS422 level convertors to the CPM expansion 
connector, P4. The signals, as numbered on the connector itself
(beware: the numbering on the PCB does not agree):

TX Data  (SCC3 TXD) (output) Pin a25
TX Clock (BRG4O)    (output) Pin a11
Rx Data  (SCC3 RXD) (input)  Pin c15
Rx Clock (CLK5)     (input)  Pin d28
Ground   (GND)      (n/a)    Pin c1


Debugging/ Code loading:
------------------------
Tested using the Metrowerks debugger and Macraigor OCDemon (Raven).  
The OCD connects via the parallel port and allows you to download code 
to the board.  It may be possible to use some other debugger if you
don't already have Metrowerks CodeWarrior.



Timing tests
------------
```
*** TESTING IN PROGRESS - DO NOT BELIEVE THESE RESULTS ***

Single processor tests:  Passed
Multi-processort tests:  not applicable
Timing tests:
	Context Switch

	context switch: self 9
	context switch: to another task 10
	context switch: no floating point contexts 23
	fp context switch: restore 1st FP task 24
	fp context switch: save initialized, restore initialized 11
	fp context switch: save idle, restore initialized 11
	fp context switch: save idle, restore idle 23

	Task Manager

	rtems_task_create 83
	rtems_task_ident 84
	rtems_task_start 30
	rtems_task_restart: calling task 48
	rtems_task_restart: suspended task -- returns to caller 36
	rtems_task_restart: blocked task -- returns to caller 47
	rtems_task_restart: ready task -- returns to caller 35
	rtems_task_restart: suspended task -- preempts caller 56 
	rtems_task_restart: blocked task -- preempts caller 116
	rtems_task_restart: ready task -- preempts caller 93
	rtems_task_delete: calling task 102
	rtems_task_delete: suspended task 74
	rtems_task_delete: blocked task 76
	rtems_task_delete: ready task 80
	rtems_task_suspend: calling task 37
	rtems_task_suspend: returns to caller 14
	rtems_task_resume: task readied -- returns to caller 16
	rtems_task_resume: task readied -- preempts caller 30
	rtems_task_set_priority: obtain current priority 12
	rtems_task_set_priority: returns to caller 23
	rtems_task_set_priority: preempts caller 52
	rtems_task_mode: obtain current mode 5
	rtems_task_mode: no reschedule 6
	rtems_task_mode: reschedule -- returns to caller 15
	rtems_task_mode: reschedule -- preempts caller 43
	rtems_task_wake_after: yield -- returns to caller 8
	rtems_task_wake_after: yields -- preempts caller 30
	rtems_task_wake_when: 49

	Interrupt Manager

	interrupt entry overhead: returns to nested interrupt 7
	interrupt entry overhead: returns to interrupted task 31
	interrupt entry overhead: returns to preempting task 14
	interrupt exit overhead: returns to nested interrupt 10
	interrupt exit overhead: returns to interrupted task 8
	interrupt exit overhead: returns to preempting task 45

	Clock Manager

	rtems_clock_set 28
	rtems_clock_get 0
	rtems_clock_tick 36

	Timer Manager

	rtems_timer_create 11
	rtems_timer_ident 82
	rtems_timer_delete: inactive 14
	rtems_timer_delete: active 16
	rtems_timer_fire_after: inactive 20
	rtems_timer_fire_after: active 22
	rtems_timer_fire_when: inactive 24
	rtems_timer_fire_when: active 24
	rtems_timer_reset: inactive 18
	rtems_timer_reset: active 21
	rtems_timer_cancel: inactive 11
	rtems_timer_cancel: active 12

	Semaphore Manager

	rtems_semaphore_create 56
	rtems_semaphore_ident 94
	rtems_semaphore_delete 34
	rtems_semaphore_obtain: available 13
	rtems_semaphore_obtain: not available -- NO_WAIT 13
	rtems_semaphore_obtain: not available -- caller blocks 48
	rtems_semaphore_release: no waiting tasks 16
	rtems_semaphore_release: task readied -- returns to caller 36
	rtems_semaphore_release: task readied -- preempts caller   36

	Message Queue Manager

	rtems_message_queue_create 110
	rtems_message_queue_ident 82
	rtems_message_queue_delete 43
	rtems_message_queue_send: no waiting tasks 28
	rtems_message_queue_send: task readied -- returns to caller 31
	rtems_message_queue_send: task readied -- preempts caller 46
	rtems_message_queue_urgent: no waiting tasks 28
	rtems_message_queue_urgent: task readied -- returns to caller 31
	rtems_message_queue_urgent: task readied -- preempts caller 46
	rtems_message_queue_broadcast: no waiting tasks 22
	rtems_message_queue_broadcast: task readied -- returns to caller 81
	rtems_message_queue_broadcast: task readied -- preempts caller 75
	rtems_message_queue_receive: available 26
	rtems_message_queue_receive: not available -- NO_WAIT 15
	rtems_message_queue_receive: not available -- caller blocks 48
	rtems_message_queue_flush: no messages flushed 14
	rtems_message_queue_flush: messages flushed 14

	Event Manager

	rtems_event_send: no task readied 12
	rtems_event_send: task readied -- returns to caller 38
	rtems_event_send: task readied -- preempts caller 21
	rtems_event_receive: obtain current events 1
	rtems_event_receive: available 19
	rtems_event_receive: not available -- NO_WAIT 11
	rtems_event_receive: not available -- caller blocks 36

	Signal Manager

	rtems_signal_catch: 31
	rtems_signal_send: returns to caller 21
	rtems_signal_send: signal to self 39
	exit ASR overhead: returns to calling task 30
	exit ASR overhead: returns to preempting task 33

	Partition Manager

	rtems_partition_create 59
	rtems_partition_ident 82
	rtems_partition_delete 20
	rtems_partition_get_buffer: available 19
	rtems_partition_get_buffer: not available 13
	rtems_partition_return_buffer 20

	Region Manager

	rtems_region_create 37
	rtems_region_ident 84
	rtems_region_delete 20
	rtems_region_get_segment: available 19
	rtems_region_get_segment: not available -- NO_WAIT 23
	rtems_region_get_segment: not available -- caller blocks 75
	rtems_region_return_segment: no waiting tasks 21
	rtems_region_return_segment: task readied -- returns to caller 55
	rtems_region_return_segment: task readied -- preempts caller 82

	Dual-Ported Memory Manager

	rtems_port_create 23
	rtems_port_ident 82
	rtems_port_delete 21
	rtems_port_internal_to_external 10
	rtems_port_external_to_internal 11

	IO Manager

	rtems_io_initialize 1
	rtems_io_open 1
	rtems_io_close 1
	rtems_io_read 1
	rtems_io_write 1
	rtems_io_control 1

	Rate Monotonic Manager

	rtems_rate_monotonic_create 43
	rtems_rate_monotonic_ident 82
	rtems_rate_monotonic_cancel 23
	rtems_rate_monotonic_delete: active 28
	rtems_rate_monotonic_delete: inactive 25
	rtems_rate_monotonic_period: obtain status 17
	rtems_rate_monotonic_period: initiate period -- returns to caller 32
	rtems_rate_monotonic_period: conclude periods -- caller blocks 30

Network tests:
       TCP throughput (as measured by ttcp):
               Receive: 1324 kbytes/sec
               Transmit: 1037 kbytes/sec
```
