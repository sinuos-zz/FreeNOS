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

#ifndef __VIRT_MEMORY_H
#define __VIRT_MEMORY_H

#include "process.h"
#include "types.h"

/**
 * @defgroup kernel kernel (generic)
 * @{
 */

/**
 * Initialize and enable virtual memory.
 */
extern void virt_memory_init(void);

extern void virt_memory_proc(process_t *proc, addr_t kern_stack, addr_t user_stack);

extern result_t virt_map(addr_t dir_base, addr_t vaddr,
            		 addr_t paddr, size_t size, u16 flags);

extern void * virt_temp_map(addr_t paddr, size_t size);

extern void virt_unmap(void *vaddr, size_t size);

extern void virt_release(addr_t dir_base);

extern page_entry_t * virt_map_remote_dir(process_t *proc);

extern void virt_unmap_remote_dir(process_t *proc);

/**
 * @}
 */

#endif /* __VIRT_MEMORY_H */

