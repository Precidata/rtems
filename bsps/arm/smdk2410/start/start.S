/*
 * SMDK2410 startup code
 */

/*
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/asm.h>
#include <rtems/score/cpu.h>

.text
.globl  _start
_start:
	b		_start2

@---------------------------------------------------------------------------------
@ AXF addresses
@---------------------------------------------------------------------------------
	.word   bsp_section_text_begin
	.word   bsp_section_rodata_end
	.word   bsp_section_data_begin
	.word   bsp_section_bss_end
	.word   bsp_section_bss_begin
	.word   bsp_section_bss_end

@---------------------------------------------------------------------------------
@ GamePark magic sequence
@---------------------------------------------------------------------------------
	.word   0x44450011
	.word   0x44450011
	.word   0x01234567
	.word   0x12345678
	.word   0x23456789
	.word   0x34567890
	.word   0x45678901
	.word   0x56789012
	.word   0x23456789
	.word   0x34567890
	.word   0x45678901
	.word   0x56789012
	.word   0x23456789
	.word   0x34567890
	.word   0x45678901
	.word   0x56789012

@---------------------------------------------------------------------------------
_start2:
@---------------------------------------------------------------------------------

        /*
         * Since I don't plan to return to the bootloader,
         * I don't have to save the registers.
         */

        /* Set end of interrupt stack area */
        ldr     r7, =_ISR_Stack_area_end

        /* Enter FIQ mode and set up the FIQ stack pointer */
        mov     r0, #(ARM_PSR_M_FIQ | ARM_PSR_I | ARM_PSR_F)
        msr     cpsr, r0
        ldr     r1, =bsp_stack_fiq_size
        mov     sp, r7
        sub     r7, r7, r1

        /* Enter ABT mode and set up the ABT stack pointer */
        mov     r0, #(ARM_PSR_M_ABT | ARM_PSR_I | ARM_PSR_F)
        msr     cpsr, r0
        ldr     r1, =bsp_stack_abt_size
        mov     sp, r7
        sub     r7, r7, r1

        /* Enter UND mode and set up the UND stack pointer */
        mov     r0, #(ARM_PSR_M_UND | ARM_PSR_I | ARM_PSR_F)
        msr     cpsr, r0
        ldr     r1, =bsp_stack_und_size
        mov     sp, r7
        sub     r7, r7, r1

        /* Enter IRQ mode and set up the IRQ stack pointer */
        mov     r0, #(ARM_PSR_M_IRQ | ARM_PSR_I | ARM_PSR_F)
        msr     cpsr, r0
        mov     sp, r7

        /*
         * Enter SVC mode and set up the SVC stack pointer, reuse IRQ stack
         * (interrupts are disabled).
         */
        mov     r0, #(ARM_PSR_M_SVC | ARM_PSR_I | ARM_PSR_F)
        msr     cpsr, r0
        mov     sp, r7

        /* Stay in SVC mode */

	/* disable mmu, I and D caches*/
	nop
	nop
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #0x01
	bic r0, r0, #0x04
	bic r0, r0, #0x01000
	mcr p15, 0, r0, c1, c0, 0
	nop
	nop

	/* clean data cache */
	mov   r1,#0x00
Loop1:
	mov   r2,#0x00
Loop2:
	mov r3, r2, lsl#26
	orr r3, r3, r1, lsl#5
	mcr p15, 0, r3, c7, c14, 2
	add r2, r2, #0x01
	cmp r2, #64
	bne Loop2
	add r1, r1, #0x01
	cmp r1, #8
	bne Loop1


        /*
         * Initialize the MMU. After we return, the MMU is enabled,
         * and memory may be remapped. I hope we don't remap this
         * memory away.
         */
        ldr     r0, =mem_map
        bl      mmu_init

        /*
         * Initialize the exception vectors. This includes the
         * exceptions vectors (0x00000000-0x0000001c), and the
         * pointers to the exception handlers (0x00000020-0x0000003c).
         */
        mov     r0, #0
        adr     r1, vector_block
        ldmia   r1!, {r2-r9}
        stmia   r0!, {r2-r9}
        ldmia   r1!, {r2-r9}
        stmia   r0!, {r2-r9}

        /* Now we are prepared to start the BSP's C code */
        mov     r0, #0
        bl      boot_card

        /*
         * Theoretically, we could return to what started us up,
         * but we'd have to have saved the registers and stacks.
         * Instead, we'll just reset.
         */
        bl      bsp_reset

        /* We shouldn't get here. If we do, hang */
_hang:  b       _hang


/*
 * This is the exception vector table and the pointers to
 * the functions that handle the exceptions. It's a total
 * of 16 words (64 bytes)
 */
vector_block:
        ldr    pc, handler_addr_reset
        ldr    pc, handler_addr_undef
        ldr    pc, handler_addr_swi
        ldr    pc, handler_addr_prefetch
        ldr    pc, handler_addr_abort
        nop
        ldr    pc, handler_addr_irq
        ldr    pc, handler_addr_fiq

handler_addr_reset:
        .word  bsp_reset

handler_addr_undef:
        .word  _ARMV4_Exception_undef_default

handler_addr_swi:
        .word  _ARMV4_Exception_swi_default

handler_addr_prefetch:
        .word  _ARMV4_Exception_pref_abort_default

handler_addr_abort:
        .word  _ARMV4_Exception_data_abort_default

handler_addr_reserved:
        .word  0

handler_addr_irq:
        .word  _ARMV4_Exception_interrupt

handler_addr_fiq:
        .word  _ARMV4_Exception_fiq_default
