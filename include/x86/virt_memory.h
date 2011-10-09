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

#ifndef __X86_VIRT_MEMORY_H
#define __X86_VIRT_MEMORY_H

/**
 * @defgroup x86kernel kernel (x86)
 * @{
 */

/** Page entry bit shift. */
#define PAGE_SHIFT      12

/** Page table bit shift. */
#define DIR_SHIFT       22

/** Size of one physical memory page. */
#define PAGE_SIZE       4096
#define TAB_SIZE	4096
#define DIR_SIZE	4096

/** Mask to find the page. */
#define PAGE_MASK        0xfffff000

/**
 * Entry inside the page directory of a given virtual address.
 * @param vaddr Virtual Address.
 * @return Index of the corresponding page directory entry.
 */
#define DIRENTRY(vaddr) \
    ((vaddr) >> DIR_SHIFT)

#define TABENTRY(vaddr) \
    (((vaddr) >> PAGE_SHIFT) & 0x3ff)

#define TABADDR(dir_base,vaddr) \
    (page_entry_t *) ((dir_base) + (((vaddr) >> DIR_SHIFT) * PAGE_SIZE))

#define LOCAL_DIR  TABADDR(DIR_VADDR, DIR_VADDR)
#define REMOTE_DIR TABADDR(RDIR_VADDR, DIR_VADDR)

/**
 * Flushes the Translation Lookaside Buffers (TLB) for a single page.
 * @param addr Memory address to flush.
 */
#define virt_reload_addr(addr) \
    __asm__ volatile("invlpg (%0)" ::"r" ((addr_t)addr) : "memory")

/**
 * Flushes all Translation Lookaside Buffers (TLB).
 */
#define virt_reload_all() \
    __asm__ volatile("mov %cr3, %eax\n" \
		     "mov %eax, %cr3\n")

/**
 * @brief Page Enter Flags.
 * @{
 */

/** Page is present. */
#define PAGE_PR     (1 << 0)

/** Page is writable. */
#define PAGE_WR     (1 << 1)

/** Page is accessibl by unprivileged userland processes. */
#define PAGE_USER   (1 << 2)

/** Pinned pages cannot be released. */
#define PAGE_PIN (1 << 9)

/**
 * @}
 */

#define DIR_VADDR	 (1024 * 1024 * 4)
#define RDIR_VADDR	 (1024 * 1024 * 8)
#define TMP_VADDR	 (1024 * 1024 * 12)
#define TMP_VADDR_MAX	 (1024 * 1024 * 16)
#define KERN_VADDR	 (1024 * 1024 * 16)

#define KERN_STACK	 (0xbfff0000)
#define USER_STACK	 (0xbffc0000)

#ifndef __ASSEMBLER__

#include "types.h"

typedef u32 page_entry_t;

#include <kernel/virt_memory.h>

#endif /* __ASSEMBLER__ */

/**
 * @}
 */

#endif /* __X86_VIRT_MEMORY_H */

