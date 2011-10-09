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

#ifndef __KERNEL_SMP_H
#define __KERNEL_SMP_H

#include "types.h"
#include "macro.h"
#include "process.h"

/** CPU is detected. */
#define SMP_AVAIL  (1 << 0)

/** No CPU detected at this slot. */
#define SMP_EMPTY  (1 << 1)

/** CPU is booted. */
#define SMP_ACTIVE (1 << 2)

/**
 * Describes a CPU connected to the system.
 */
typedef struct cpu
{
/*    char type[32];*/ /** Full name of the CPU type. */
    uint id;	     /** Unique ID number. */
    u8 flags;	     /** CPU flags. */
    addr_t stack;    /** Address of the initial CPU stack. */
    process_t *proc; /** Currently running process. */
    process_t *idle; /** Run this process if no other is ready. */
}
PACKED cpu_t;

/**
 * Initialize the SMP code.
 */
extern void smp_init(void);

/**
 * Startup all CPU's.
 */
extern void smp_boot_all(void);

/**
 * See if the current processor is the boot CPU.
 * @return Non-zero if BSP, and zero otherwise.
 */
extern uint smp_is_bootcpu(void);

/**
 * Retrieve the unique ID of the current processor.
 * @return Processor number.
 */
extern uint smp_id(void);

/**
 * Retrieve a pointer to the processor_t of the current CPU.
 * @return cpu_t structure pointer.
 */
extern cpu_t * smp_cpu(void);

/** Array of known CPU's on the system. */
extern cpu_t cpus[SMP_MAX];

/** Number of CPU's on the system in total. */
extern uint cpus_total;

#endif /* __KERNEL_SMP_H */

