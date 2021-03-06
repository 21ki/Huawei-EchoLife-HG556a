/*
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999 by Helge Deller
 * Copyright 1999 SuSE GmbH (Philipp Rumpf)
 * Copyright 1999 Philipp Rumpf (prumpf@tux.org)
 *
 * Initial Version 04-23-1999 by Helge Deller <deller@gmx.de>
 */

#include <linux/autoconf.h>	/* for CONFIG_SMP */

#include <asm/offsets.h>
#include <asm/psw.h>

#include <asm/assembly.h>
#include <asm/pgtable.h>


	.level 1.1

	.data

	.export boot_args
boot_args:
	.word 0 /* arg0 */
	.word 0 /* arg1 */
	.word 0 /* arg2 */
	.word 0 /* arg3 */

	.text
	.align	4	
	.import init_thread_union,data
	.import	$global$		/* forward declaration */
	.import fault_vector_11,code	/* IVA parisc 1.1 32 bit */
	.import fault_vector_20,code	/* IVA parisc 2.0 32 bit */

	.export stext
	.export _stext,data		/* Kernel want it this way! */
_stext:
stext:
	.proc
	.callinfo

	/* Make sure sr4-sr7 are set to zero for the kernel address space */
	mtsp	%r0,%sr4
	mtsp	%r0,%sr5
	mtsp	%r0,%sr6
	mtsp	%r0,%sr7

	/* Clear BSS (shouldn't the boot loader do this?) */

	.import __bss_start,data
	.import __bss_stop,data

	ldil            L%PA(__bss_start),%r3
	ldo             R%PA(__bss_start)(%r3),%r3
	ldil            L%PA(__bss_stop),%r4
	ldo             R%PA(__bss_stop)(%r4),%r4
$bss_loop:
	cmpb,<<,n       %r3,%r4,$bss_loop
	stw,ma          %r0,4(%r3)

	/* Save away the arguments the boot loader passed in (32 bit args) */

	ldil            L%PA(boot_args),%r1
	ldo             R%PA(boot_args)(%r1),%r1
	stw,ma          %arg0,4(%r1)
	stw,ma          %arg1,4(%r1)
	stw,ma          %arg2,4(%r1)
	stw,ma          %arg3,4(%r1)

	/* Initialize startup VM. Just map first 8 MB of memory */
	ldil		L%PA(pg0),%r1
	ldo		R%PA(pg0)(%r1),%r1
	shr		%r1,PxD_VALUE_SHIFT,%r3
	ldo		(PxD_FLAG_PRESENT+PxD_FLAG_VALID)(%r3),%r3

	ldil		L%PA(swapper_pg_dir),%r4
	ldo		R%PA(swapper_pg_dir)(%r4),%r4
	mtctl		%r4,%cr24	/* Initialize kernel root pointer */
	mtctl		%r4,%cr25	/* Initialize user root pointer */
	ldi		ASM_PT_INITIAL,%r1
	ldo		ASM_PGD_ENTRY*ASM_PGD_ENTRY_SIZE(%r4),%r4
1:
	stw             %r3,0(%r4)
	ldo		(ASM_PAGE_SIZE >> PxD_VALUE_SHIFT)(%r3),%r3
	addib,>		-1,%r1,1b
	ldo		ASM_PGD_ENTRY_SIZE(%r4),%r4

	ldo		_PAGE_KERNEL(%r0),%r3 /* Hardwired 0 phys addr start */
	ldil		L%PA(pg0),%r1
	ldo		R%PA(pg0)(%r1),%r1
$pgt_fill_loop:
	stwm		%r3,ASM_PTE_ENTRY_SIZE(%r1)
	ldo		ASM_PAGE_SIZE(%r3),%r3
	bb,>=		%r3,31-KERNEL_INITIAL_ORDER,$pgt_fill_loop
	nop


	/* Load the return address...er...crash 'n burn */
	copy		%r0,%r2

	/* And the RFI Target address too */
	ldil            L%start_kernel,%r11
	ldo             R%start_kernel(%r11),%r11

	/* And the initial task pointer */

	ldil            L%init_thread_union,%r6
	ldo             R%init_thread_union(%r6),%r6
	mtctl           %r6,%cr30

	/* And the stack pointer too */

	ldo             THREAD_SZ_ALGN(%r6),%sp

	/* And the interrupt stack */

	ldil            L%interrupt_stack,%r6
	ldo             R%interrupt_stack(%r6),%r6
	mtctl           %r6,%cr31

#ifdef CONFIG_SMP
	/* Set the smp rendevous address into page zero.
	** It would be safer to do this in init_smp_config() but
	** it's just way easier to deal with here because
	** of 64-bit function ptrs and the address is local to this file.
	*/
	ldil		L%PA(smp_slave_stext),%r10
	ldo		R%PA(smp_slave_stext)(%r10),%r10
	stw		%r10,0x10(%r0)	/* MEM_RENDEZ */
	stw		%r0,0x28(%r0)	/* MEM_RENDEZ_HI - assume addr < 4GB */

	/* FALLTHROUGH */
	.procend

	/*
	** Code Common to both Monarch and Slave processors.
	** Entry:
	**    %r11 must contain RFI target address.
	**    %r25/%r26 args to pass to target function
	**    %r2  in case rfi target decides it didn't like something
	**
	** Caller must init: SR4-7, %sp, %r10, %cr24/25, 
	*/
common_stext:
	.proc
	.callinfo
#else
	/* Clear PDC entry point - we won't use it */
	stw		%r0,0x10(%r0)	/* MEM_RENDEZ */
	stw		%r0,0x28(%r0)	/* MEM_RENDEZ_HI */
#endif

	/* PARANOID: clear user scratch/user space SR's */
	mtsp	%r0,%sr0
	mtsp	%r0,%sr1
	mtsp	%r0,%sr2
	mtsp	%r0,%sr3

	/* Initialize Protection Registers */
	mtctl	%r0,%cr8
	mtctl	%r0,%cr9
	mtctl	%r0,%cr12
	mtctl	%r0,%cr13

	/* Initialize the global data pointer */
	ldil		L%$global$,%dp
	ldo		R%$global$(%dp),%dp

	/*
	 * Set up our interrupt table.  HPMCs might not work after this! 
	 *
	 * We need to install the correct iva for PA1.1 or PA2.0. The
	 * following short sequence of instructions can determine this
	 * (without being illegal on a PA1.1 machine).
	 */
	ldi		32,%r10
	mtctl		%r10,%cr11
	.level 2.0
	mfctl,w		%cr11,%r10
	.level 1.1
	comib,<>,n	0,%r10,$is_pa20
	ldil		L%PA(fault_vector_11),%r10
	b		$install_iva
	ldo		R%PA(fault_vector_11)(%r10),%r10

$is_pa20:
	ldil		L%PA(fault_vector_20),%r10
	ldo		R%PA(fault_vector_20)(%r10),%r10

$install_iva:
	mtctl		%r10,%cr14

	/* Disable Q bit so we can load the iia queue */
	rsm            PSW_SM_Q,%r0

	/* kernel PSW:
	 *  - no interruptions except HPMC and TOC (which are handled by PDC)
	 *  - Q bit set (IODC / PDC interruptions)
	 *  - big-endian
	 *  - virtually mapped
	 */
	ldil		L%KERNEL_PSW,%r10
	ldo		R%KERNEL_PSW(%r10),%r10
	mtctl		%r10,%ipsw

	/* Set the space pointers for the post-RFI world
	** Clear the two-level IIA Space Queue, effectively setting
	** Kernel space.
	*/
	mtctl		%r0,%cr17	/* Clear IIASQ tail */
	mtctl		%r0,%cr17	/* Clear IIASQ head */

	/* Load RFI target into PC queue */
	mtctl		%r11,%cr18	/* IIAOQ head */
	ldo		4(%r11),%r11
	mtctl		%r11,%cr18	/* IIAOQ tail */

	/* Jump to hyperspace */
	rfi
	nop

	.procend

#ifdef CONFIG_SMP

	.import smp_init_current_idle_task,data
	.import	smp_callin,code

smp_callin_rtn:
        .proc
	.callinfo
	break	1,1		/*  Break if returned from start_secondary */
	nop
	nop
        .procend

/***************************************************************************
*
* smp_slave_stext is executed by all non-monarch Processors when the Monarch
* pokes the slave CPUs in smp.c:smp_boot_cpus().
*
* Once here, registers values are initialized in order to branch to virtual
* mode. Once all available/eligible CPUs are in virtual mode, all are
* released and start out by executing their own idle task.
*****************************************************************************/


smp_slave_stext:
        .proc
	.callinfo

	/*
	** Initialize Space registers
	*/
	mtsp	   %r0,%sr4
	mtsp	   %r0,%sr5
	mtsp	   %r0,%sr6
	mtsp	   %r0,%sr7

	/*  Initialize the SP - monarch sets up smp_init_current_idle_task */
	ldil		L%PA(smp_init_current_idle_task),%sp
	ldo		R%PA(smp_init_current_idle_task)(%sp),%sp
	ldw		0(%sp),%sp	/* load task address */
	tophys_r1	%sp
	ldw		TASK_THREAD_INFO(%sp), %sp
	mtctl           %sp,%cr30       /* store in cr30 */
	addil		L%THREAD_SZ_ALGN,%sp	/* stack is above task */
	ldo		R%THREAD_SZ_ALGN(%r1),%sp

	/* point CPU to kernel page tables */
	ldil		L%PA(swapper_pg_dir),%r4
	ldo		R%PA(swapper_pg_dir)(%r4),%r4
	mtctl		%r4,%cr24	/* Initialize kernel root pointer */
	mtctl		%r4,%cr25	/* Initialize user root pointer */

	/* Load RFI *return* address in case smp_callin bails */
	ldil		L%smp_callin_rtn,%r2
	ldo		R%smp_callin_rtn(%r2),%r2

	/* Load RFI target address.  */
	ldil		L%smp_callin,%r11
	ldo		R%smp_callin(%r11),%r11
	
	/* ok...common code can handle the rest */
	b		common_stext
	nop

	.procend
#endif /* CONFIG_SMP */

	.data

	.align	4
	.export	$global$,data

	.type	$global$,@object
	.size	$global$,4
$global$:	
	.word 0
