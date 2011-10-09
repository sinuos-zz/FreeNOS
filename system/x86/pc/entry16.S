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

#include <FreeNOS/multiboot.h>
#include <FreeNOS/cpu.h>

/* Export symbols. */
.global smp_entry

/*.section ".text"*/
.data
.code16

/*
 * 16-bit real mode entry point for each newly booted CPU.
 */
smp_entry:
base = .

    /* Disable interrupts. */
    cli

    /* Setup segment registers. */
    mov %cs, %ax
    mov %ax, %ds

    /* Load the global descriptor table. */
    lgdtl gdt16_ptr - base

    /* Enable Protected mode. */
    movl %cr0, %eax
    orl  $(CR0_PE), %eax
    movl %eax, %cr0

    /* Reload data segment. */
    mov $KERN_DS_SEL, %ax
    mov %ax, %ds

    /* Reload code segment: jump to 32-bit entry point. */
    ljmpl $KERN_CS_SEL, $loader

gdt16:
        .quad   0x0000000000000000 /* NULL descriptor. */
        .quad   0x00cf9a000000ffff /* Kernel CS. */
        .quad   0x00cf92000000ffff /* Kernel DS. */
        .quad   0x00cffa000000ffff /* User CS. */
        .quad   0x00cff2000000ffff /* User DS. */
        .quad   0x0000000000000000 /* TSS descriptor. */
gdt16_end:

gdt16_ptr:
        .word gdt16_end - gdt16
        .long gdt16
        .word 0

