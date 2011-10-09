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

#include <FreeNOS/irq_table.h>
#include <FreeNOS/cpu.h>

IRQ(0)    IRQ(1)    IRQ(2)    IRQ(3)    IRQ(4)
IRQ(5)    IRQ(6)    IRQ(7)    IRQ_E(8)  IRQ(9)
IRQ_E(10) IRQ_E(11) IRQ_E(12) IRQ_E(13) IRQ_E(14)
IRQ(15)   IRQ(16)   IRQ(32)   IRQ(33)   IRQ(34)
IRQ(35)   IRQ(36)   IRQ(37)   IRQ(38)   IRQ(39)
IRQ(40)   IRQ(41)   IRQ(42)   IRQ(43)   IRQ(44)
IRQ(45)   IRQ(46)   IRQ(47)   IRQ(48)	IRQ(144)

irq_execute:

    /* Make a cpu_state_t. */
    pusha
    pushl %ss
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    /* Replace data segments. */
    pushl %eax
    mov $KERN_DS_SEL, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    popl %eax

    /* Invoke the IRQ handler. */
    call irq_raise_handler

    /* Restore data segments. */
    popl %gs
    popl %fs
    popl %es
    popl %ds
    add $4, %esp

    /* Continue program. */
    popa
    add $8, %esp
    iret

