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

#ifndef __CPU_H
#define __CPU_H

/**
 * @defgroup x86kernel kernel (x86)
 * @{
 */

/** Intel is little endian. */
#define CPU_LITTLE_ENDIAN 1

/** Protected Mode. */
#define CR0_PE		0x00000001

/** Paged Mode. */
#define CR0_PG		0x80000000

/** Timestamp Counter Disable. */
#define CR4_TSD		0x00000004

/** Kernel Code Segment. */
#define KERN_CS       1
#define KERN_CS_SEL   0x8

/** System Data Segment. */
#define KERN_DS       2
#define KERN_DS_SEL   0x10

/** User Code Segment. */
#define USER_CS         3
#define USER_CS_SEL     (0x18 | 3)

/** User Data Segment. */
#define USER_DS         4
#define USER_DS_SEL     (0x20 | 3)

/** Task State Segment. */
#define KERN_TSS        5
#define KERN_TSS_SEL    0x28

/* The size of our stack (16KB).  */
#define STACK_SIZE 0x4000

/* Maximum number of IRQ vectors. */
#define MAX_IRQ 256

#ifndef __ASSEMBLER__

#include "types.h"

#define get_eflags(...) \
({ \
    u32 eflags; \
    __asm__ volatile("pushfl ; popl %0" : "=r" (eflags)); \
    eflags; \
})

#define set_eflags(eflags) \
({ \
    __asm__ volatile("pushl %0; popfl" : : "r" (eflags)); \
})

#define set_cr3(addr) \
({ \
    __asm__ volatile ("pushl %%eax;" \
		  "movl %0, %%eax;" \
		  "movl %%eax, %%cr3;" \
		  "popl %%eax;" : : "m" (addr)); \
})

#define add_cr0(flags) \
({ \
    __asm__ volatile ("pushl %%eax;" \
		  "movl %%cr0, %%eax;" \
		  "orl %0, %%eax;" \
		  "movl %%eax, %%cr0;" \
		  "popl %%eax" : : "d" (flags)); \
})

/**
 * Load Interrupt Descriptor Table.
 * @param ptr idt_ptr_t structure.
 * @see idt.h
 */
#define lidt(ptr) \
    __asm__ volatile("lidt %0" : : "m" ((ptr)))

/**
 * Clear interrupts off.
 */
#define cli() \
    __asm__ volatile("cli")

/**
 * Set interrupts on.
 */
#define sti() \
    __asm__ volatile("sti")

/**
 * Loads the Task State Register (LTR) with the given segment.
 * @param sel TSS segment selector.
 */
#define ltr(sel) \
({ \
    u16 tr = sel; \
    __asm__ volatile ("ltr %0\n" :: "r"(tr)); \
})

/**
 * Puts the CPU in a lower power consuming state.
 */
#define idle() \
    __asm__ volatile ("hlt");

typedef struct cpu_state
{
    /* Segments. */
    u32 gs, fs, es, ds, ss0;

    /* By pusha */
    u32 edi, esi, ebp, esp0, ebx, edx, ecx, eax;

    /* Vector/error arguments. */
    u32 vector, error;

    /* Pushed by processor. */
    u32 eip, cs, eflags, esp3, ss3;
}
cpu_state_t;

#endif /* __ASSEMBLER__ */

/**
 * @}
 */

#endif /* __CPU_H */

