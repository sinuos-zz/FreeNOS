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
 *
 */

#include <FreeNOS/virt_memory.h>
#include <FreeNOS/cpu.h>

.global gdt_load

.section ".text"
.code32

/*
 * gdt_load(gdt_ptr_t *ptr)
 */
gdt_load:

    /* Disable interrupts. */
    cli

    /* Load GDT. */
    movl 4(%esp), %eax
    lgdtl (%eax)

    /* Setup segments. */
    movl $KERN_DS_SEL, %edx
    movl %edx, %ds
    movl %edx, %es
    movl %edx, %fs
    movl %edx, %gs

    /* Reload CS. */
    ljmpl $KERN_CS_SEL, $gdt_load2

gdt_load2:
    ret
