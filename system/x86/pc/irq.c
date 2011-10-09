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

#include <FreeNOS/irq.h>
#include <FreeNOS/idt.h>
#include <FreeNOS/pic.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/api.h>
#include <FreeNOS/printk.h>

static const char *exceptions[] = {
    /*  0 */ "Divide Error Exception",
    /*  1 */ "Debug Exception",
    /*  2 */ "NMI Interrupt",
    /*  3 */ "Breakpoint Exception",
    /*  4 */ "Overflow Exception",
    /*  5 */ "BOUND Range Exceeded Exception",
    /*  6 */ "Invalid Opcode Exception",
    /*  7 */ "Device Not Available Exception",
    /*  8 */ "Double Fault Exception",
    /*  9 */ "Coprocessor Segment Overrun",
    /* 10 */ "Invalid TSS Exception",
    /* 11 */ "Segment Not Present",
    /* 12 */ "Stack Fault Exception",
    /* 13 */ "General Protection Exception",
    /* 14 */ "Page-Fault Exception",
    /* 15 */ "Unknown Exception",
    /* 16 */ "x87 FPU Floating-Point Error",
    /* 17 */ "Alignment Check Exception",
    /* 18 */ "Machine-Check Exception",
    /* 19 */ "SIMD Floating-Point Exception",
};

irq_handler_t irq_handler[MAX_IRQ];

void irq_raise_handler(cpu_state_t state)
{
    if (state.vector < MAX_IRQ && irq_handler[state.vector] != 0)
    {
	irq_handler[state.vector](&state);
    }
    else
	printk("Ignoring unhandled IRQ %x:%x\r\n",
		state.vector, state.error);
}

void irq_ignore(cpu_state_t *state)
{
    printk("CPU#%d: interrupt vector %x:%x raised (ignored)\r\n",
	    smp_id(), state->vector, state->error);
}

void exception(cpu_state_t *state)
{
    printk("CPU#%d: IRQ %x:%x `%s' raised (PID %l)\r\n"
	   " EAX=%8x EBX=%8x ECX=%8x EDX=%8x\r\n"
	   " ESI=%8x EDI=%8x EBP=%8x ESP=%8x\r\n"
	   " EIP=%8x EFLAGS=%8x\r\n"
	   " CS=%4x SS=%4x DS=%4x ES=%4x FS=%4x GS=%4x\r\n",
	    smp_id(), state->vector, state->error, exceptions[state->vector], proc_lookup(smp_cpu()->proc),
	    state->eax, state->ebx, state->ecx, state->edx,
	    state->esi, state->edi, state->ebp, state->esp0,
	    state->eip, state->eflags,
	    state->cs, state->ss0, state->ds, state->es, state->fs, state->gs);
}

void trap(cpu_state_t *state)
{
    state->eax = api_execute(state->eax, state->ecx,
			     state->ebx, state->edx);
}

void irq_init(void)
{
    int i;

    /* Initiate list of IRQ handlers. */
    for (i = 0; i < MAX_IRQ; i++)
    {
	if (i <= 15)
	    irq_handler[i] = exception;
	else if (i == 0x90)
	    irq_handler[i] = trap;
	else
	    irq_handler[i] = irq_ignore;
    }
    /* Initialize PIC and IDT. */
    pic_init();
    idt_init();
}

void irq_install(u16 irq, irq_handler_t handler)
{
    irq_handler[irq] = handler;
}

void irq_prepare(void)
{
    idt_load();
}

void irq_enable(void)
{
    sti();
}

void irq_disable(void)
{
    cli();
}

