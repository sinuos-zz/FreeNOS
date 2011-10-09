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

#ifndef __KERNEL_PROCESS_H
#define __KERNEL_PROCESS_H

/** Maximum number of processes. */
#define PROC_MAX 4096

/** Process exists. */
#define PROC_AVAIL   (1 << 0)

/** Process is ready-to-run. */
#define PROC_READY   (1 << 1)

/** Process is running on a processor. */
#define PROC_RUNNING (1 << 2)

#ifndef __ASSEMBLER__

#include "types.h"
#include "macro.h"

/**
 * Process structure.
 */
typedef struct process
{
    addr_t page_dir;	 /**<< Virtual memory page directory. */
    addr_t stack;	 /**<< Pointer to the last know virtual address of the stack. */
    u8 flags;		 /**<< Process flags. */
    /* pid_t parent;*/	 /**<< Only appropriate for threads: a parent. */
}
PACKED process_t;

extern void proc_table_init(void);
extern u16 proc_create(addr_t entry);
extern process_t * proc_find(u16 pid);
extern u16 proc_lookup(process_t *proc);
extern void proc_resume(process_t *proc);
extern void proc_pause(process_t *proc);
extern result_t proc_destroy(process_t *proc);
extern void proc_schedule(void);

#endif /* __ASSEMBLER__ */
#endif /* __KERNEL_PROCESS_H */

