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
 * @defgroup mipskernel kernel (mips)
 * @{
 */

/** Size of one physical memory page. */
#define PAGESIZE	4096

/** Mask to find the page. */
#define PAGEMASK        0xfffff000

/** Page entry bit shift. */
#define PAGESHIFT       12

/** Amount of memory reserved for the kernel (4MB). */
#define KERNEL_MEMORY	(1024 * 1024 * 4)

/* The size of our stack (16KB).  */
#define STACK_SIZE 0x4000

// TODO: fix this in MIPS
#define MAX_IRQ 256

#ifndef __ASSEMBLER__
#include "types.h"

// TODO: fill in all MIPS registers please
typedef struct cpu_state
{
    u32 regs;
}
cpu_state_t;

#endif /* __ASSEMBLER__ */

/**
 * @}
 */

#endif /* __CPU_H */

