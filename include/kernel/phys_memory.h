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

#ifndef __PHYS_MEMORY_H
#define __PHYS_MEMORY_H
#ifndef __ASSEMBLER__

#include "types.h"

/**
 * @defgroup kernel kernel (generic)
 * @{
 */

/**
 * Setup the physical memory map and kernel heap.
 */
extern void phys_memory_init(void);

/**
 * Allocate physical memory pages.
 * @param sz Amount of memory to allocate in bytes.
 * @return Physical address of the allocated memory.
 */
extern addr_t palloc(size_t sz);

/**
 * Allocate physical memory pages.
 * @param base Base physical address to start allocating, if possible.
 * @param sz Amount of memory to allocate in bytes.
 * @return Physical address of the allocated memory.
 */
extern addr_t palloc_from(addr_t base, size_t sz);

/**
 * Release a physical memory page.
 * @param paddr Physical address of the memory page to unmark.
 */
extern void pfree(addr_t paddr);

/**
 * Marks all physical pages used by a process as free (if not pinned).
 * @param proc Target process.
 */
extern void prelease(pid_t proc);

/** Begin and end addresses of the kernel. */
extern addr_t kernel_start, kernel_end;

/** Begin and end addresses of the boot image. */
extern addr_t boot_img_start, boot_img_end;

/** Total size of the in-memory kernel, including memory map. */
extern size_t kernel_size;

/**
 * @}
 */

#endif /* __ASSEMBLY__ */
#endif /* __PHYS_MEMORY_H */

