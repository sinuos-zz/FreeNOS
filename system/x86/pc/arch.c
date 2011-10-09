/*
 * Copyright (C) 2010 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FreeNOS/arch.h>
#include <FreeNOS/multiboot.h>
#include <FreeNOS/types.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/pit.h>
#include <FreeNOS/gdt.h>
#include <FreeNOS/tss.h>
#include <FreeNOS/io.h>
#include <FreeNOS/irq.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/apic.h>
#include <FreeNOS/virt_memory.h>
#include <FreeNOS/context_switch.h>
#include <string.h>

static int apic_timer_ic = 0;

size_t arch_total_memory(void)
{
    return (multiboot_info->mem_lower +
	    multiboot_info->mem_upper) * 1024;
}

void arch_init_proc(process_t *proc, addr_t entry,
		    addr_t kern_stack, addr_t user_stack)
{
    u8 *tmp_stack = virt_temp_map(kern_stack, STACK_SIZE);
    cpu_state_t *regs = (cpu_state_t *) (tmp_stack + STACK_SIZE - sizeof(cpu_state_t));

    /* Point stack. */
    proc->stack = KERN_STACK + STACK_SIZE - sizeof(cpu_state_t);

    /* Clear registers. */
    memset(regs, 0, sizeof(*regs));

    /* Initialize registers. */
    regs->eip  = entry;
    regs->ss0  = KERN_DS_SEL;
    regs->ss3  = USER_DS_SEL;
    regs->esp0 = KERN_STACK + STACK_SIZE;
    regs->esp3 = USER_STACK + STACK_SIZE;
    regs->ebp  = USER_STACK + STACK_SIZE;
    regs->fs   = USER_DS_SEL;
    regs->gs   = USER_DS_SEL;
    regs->es   = USER_DS_SEL;
    regs->ds   = USER_DS_SEL;
    regs->cs   = USER_CS_SEL;
    regs->eflags = 0x202;

    /* Release mapping. */
    virt_unmap(tmp_stack, STACK_SIZE);
}

uint arch_get_pit_count(void)
{
    uint count = 0;

    outb(PIT_CMD, 0);
    count  = inb(PIT_CHAN0);
    count |= inb(PIT_CHAN0) << 8;

    return count;
}

void arch_wait_pit(void)
{
    uint previous, current;

    current = arch_get_pit_count();
    do
    {
	previous = current;
	current  = arch_get_pit_count();
    }
    while (previous >= current);
}

void arch_init_pit(void)
{
    outb(PIT_CMD, 0);
    outb(PIT_CHAN0, PIT_DIVISOR & 0xff);
    outb(PIT_CHAN0, PIT_DIVISOR >> 8);
}

void arch_init_apic_timer(uint bus_counter)
{
    apic_write(APIC_REG_DCR, APIC_TIMER_DIV16);
    apic_write(APIC_REG_IC, bus_counter);
    apic_write(APIC_REG_CC, bus_counter);
    apic_write(APIC_REG_TIMER, APIC_TIMER_VECTOR | APIC_TIMER_PERIODIC);
}

void arch_apic_timer_irq(cpu_state_t *state)
{
    apic_write(APIC_REG_EOI, 0);
    proc_schedule();
}

void arch_init_clock(void)
{
    long t1, t2, i;
    int LOOPS = PIT_HZ / 10;

    /* Start the APIC and PIT timers. */
    arch_init_apic_timer(0xffffffff);
    arch_init_pit();
    arch_wait_pit();

    /* Measure the speed of the APIC timer. */
    t1 = apic_read(APIC_REG_CC);
    for (i = 0; i < LOOPS; i++)
    {
	arch_wait_pit();
    }
    t2 = apic_read(APIC_REG_CC);

    /* Calculate APIC timer value for PIT_HZ. */
    apic_timer_ic = ((t1 - t2) * 16) / LOOPS;

    /* Finally install IRQ handler. */
    irq_install(APIC_TIMER_VECTOR, arch_apic_timer_irq);

    printk("Detected %l.%l Mhz APIC bus\r\n",
	    apic_timer_ic/(1000000/PIT_HZ),
	    apic_timer_ic%(1000000/PIT_HZ));
}

void arch_enable_clock(void)
{
    apic_enable();
    arch_init_apic_timer(apic_timer_ic);
}

void arch_init_cpu(void)
{
    smp_cpu()->flags |= SMP_ACTIVE;
    gdt_init();
    tss_init();
}

void arch_context_switch(process_t *old, process_t *new)
{
    /* Update the Task State Segment.. */
    tss_update(KERN_STACK + STACK_SIZE, KERN_DS_SEL);

    /* Perform Context Switch. */
    context_switch(old ? &old->stack : NULL,
		   old ? &old->flags : NULL,
		   new->page_dir,
		   new->stack);
}
