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
#include <FreeNOS/virt_memory.h>

/* Export symbols. */
.global multiboot_entry, smp_entry, multiboot_info, stackptr, loader

.section ".boot"
/*
 * Multiboot header, 32-bit aligned.
 */
.align 4
multiboot_header:
        .long  (MULTIBOOT_HEADER_MAGIC)
        .long  (MULTIBOOT_HEADER_FLAGS)
        .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
        .long  (multiboot_header)

.section ".text"
.code32

/* Entry point for multiboot kernels. */
multiboot_entry:
    movl $stack, %edx
    addl $STACK_SIZE-1, %edx
    movl %edx, stackptr
    jmp loader

/*
 * Enter the kernel main() in 32-bit protected mode.
 */
loader:
    /* Disable interrupts. */
    cli

    /* Load GDT. */
    lgdtl gdt_ptr

    /* Setup segments. */
    movl $KERN_DS_SEL, %edx
    movl %edx, %ds
    movl %edx, %es
    movl %edx, %fs
    movl %edx, %gs

    /* Reload CS. */
    ljmpl $KERN_CS_SEL, $loader2

loader2:
    /* Setup stack. */
    lss stackptr, %esp

    /* Setup multiboot information. */
    movl %ebx, multiboot_info

    /* Enter kernel. */
    call main

hang:
    cli
    hlt
    jmp hang

.section ".data"

/* Multiboot information structure pointer. */
multiboot_info: .long 0

/* Kernel stack. */
.align PAGE_SIZE
stack:    .fill STACK_SIZE, 1, 0
stackptr: .long 0
	  .long KERN_DS_SEL

/* Boot Global Descriptor Table. */
gdt:
        .quad   0x0000000000000000 /* NULL descriptor. */
        .quad   0x00cf9a000000ffff /* Kernel CS. */
        .quad   0x00cf92000000ffff /* Kernel DS. */
        .quad   0x00cffa000000ffff /* User CS. */
        .quad   0x00cff2000000ffff /* User DS. */
        .quad   0x0000000000000000 /* TSS descriptor. */
gdt_end:

gdt_ptr:
        .word gdt_end - gdt
        .long gdt
        .word 0
