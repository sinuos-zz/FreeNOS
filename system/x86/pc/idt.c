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

#include <FreeNOS/idt.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/irq_table.h>
#include <string.h>

static idt_entry_t idt[IDT_NUM];

void idt_init(void)
{
    /* Clear the IDT. */
    memset(idt, 0, sizeof(idt));

    /* Fill the IDT with exception handlers. */
    idt_set_entry(0,  IDT_TRAP_GATE, (addr_t) i0);
    idt_set_entry(1,  IDT_TRAP_GATE, (addr_t) i1);
    idt_set_entry(2,  IDT_TRAP_GATE, (addr_t) i2);
    idt_set_entry(3,  IDT_TRAP_GATE, (addr_t) i3);
    idt_set_entry(4,  IDT_TRAP_GATE, (addr_t) i4);
    idt_set_entry(5,  IDT_TRAP_GATE, (addr_t) i5);
    idt_set_entry(6,  IDT_TRAP_GATE, (addr_t) i6);
    idt_set_entry(7,  IDT_TRAP_GATE, (addr_t) i7);
    idt_set_entry(8,  IDT_TRAP_GATE, (addr_t) i8);
    idt_set_entry(9,  IDT_TRAP_GATE, (addr_t) i9);
    idt_set_entry(10, IDT_TRAP_GATE, (addr_t) i10);
    idt_set_entry(11, IDT_TRAP_GATE, (addr_t) i11);
    idt_set_entry(12, IDT_TRAP_GATE, (addr_t) i12);
    idt_set_entry(13, IDT_TRAP_GATE, (addr_t) i13);
    idt_set_entry(14, IDT_TRAP_GATE, (addr_t) i14);
    idt_set_entry(15, IDT_TRAP_GATE, (addr_t) i15);
    idt_set_entry(16, IDT_TRAP_GATE, (addr_t) i16);

    /* Fill IDT with hardware interrupt handlers. */
    idt_set_entry(32, IDT_INT_GATE,  (addr_t) i32);
    idt_set_entry(33, IDT_INT_GATE,  (addr_t) i33);
    idt_set_entry(34, IDT_INT_GATE,  (addr_t) i34);
    idt_set_entry(35, IDT_INT_GATE,  (addr_t) i35);
    idt_set_entry(36, IDT_INT_GATE,  (addr_t) i36);
    idt_set_entry(37, IDT_INT_GATE,  (addr_t) i37);
    idt_set_entry(38, IDT_INT_GATE,  (addr_t) i38);
    idt_set_entry(39, IDT_INT_GATE,  (addr_t) i39);
    idt_set_entry(40, IDT_INT_GATE,  (addr_t) i40);
    idt_set_entry(41, IDT_INT_GATE,  (addr_t) i41);
    idt_set_entry(42, IDT_INT_GATE,  (addr_t) i42);
    idt_set_entry(43, IDT_INT_GATE,  (addr_t) i43);
    idt_set_entry(44, IDT_INT_GATE,  (addr_t) i44);
    idt_set_entry(45, IDT_INT_GATE,  (addr_t) i45);
    idt_set_entry(46, IDT_INT_GATE,  (addr_t) i46);
    idt_set_entry(47, IDT_INT_GATE,  (addr_t) i47);
    idt_set_entry(48, IDT_INT_GATE,  (addr_t) i48);
    idt_set_entry(0x90, IDT_SYS_GATE, (addr_t) i144);
}

void idt_load(void)
{
    idt_ptr_t ptr;

    /* Setup IDT descriptor. */
    ptr.limit = (IDT_NUM * sizeof(idt_entry_t)) - 1;
    ptr.base  = (u32) &idt;

    /* Load IDT. */
    lidt(ptr);
}

void idt_set_entry(u16 vector, u8 flags, addr_t isr)
{
    idt[vector].offset_low  = isr & 0xffff;
    idt[vector].segment     = KERN_CS_SEL;
    idt[vector].reserved    = 0;
    idt[vector].flags       = flags;
    idt[vector].offset_high = (isr >> 16) & 0xffff;
}

