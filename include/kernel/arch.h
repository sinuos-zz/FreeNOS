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

#ifndef __ARCH_H
#define __ARCH_H

#include "process.h"
#include "types.h"

/**
 * Determine the amount of memory available on the system.
 * @return Amount of memory in bytes.
 */
size_t arch_total_memory(void);

extern void arch_init_proc(process_t *proc, addr_t entry,
			   addr_t kern_stack, addr_t user_stack);

extern void arch_context_switch(process_t *old, process_t *new);

extern void arch_init_clock(void);
extern void arch_enable_clock(void);
extern void arch_init_cpu(void);

#endif /* __ARCH_H */

