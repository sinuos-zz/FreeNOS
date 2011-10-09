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

#ifndef __KERNEL_MULTIBOOT_H
#define __KERNEL_MULTIBOOT_H

/**
 * @defgroup kernel kernel (generic)
 * @{
 */

/** The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC		0x1BADB002

/** The flags for the Multiboot header. */
#define MULTIBOOT_HEADER_FLAGS		0x00000003

/** Size of the multiboot header structure. */
#define MULTIBOOT_HEADER_SIZE		52

/** The magic number passed by a Multiboot-compliant boot loader.  */
#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

/* Do not include in assembler source. */
#ifndef __ASSEMBLER__

#include "types.h"

/**
 * The symbol table for a.out.
 */
typedef struct aout_table
{
    u32 tab_size;
    u32 str_size;
    u32 address;
    u32 reserved;
}
aout_table_t;

/**
 * The section header table for ELF.
 */
typedef struct elf_table
{
    u32 num;
    u32 size;
    u32 address;
    u32 shndx;
}
elf_table_t;

/**
 * The Multiboot information.
 */
typedef struct multiboot_info
{
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    u32 cmdline;
    u32 mods_count;
    u32 mods_address;

    union
    {
	aout_table_t aout;
	elf_table_t elf;
    } symbols;
    u32 mmap_length;
    u32 mmap_address;
}
multiboot_info_t;

/**
 * Multiboot module information.
 */
typedef struct multiboot_module
{
    u32 mod_start;
    u32 mod_end;
    u32 string;
    u32 reserved;
}
multiboot_module_t;

/**
 * The memory map. Be careful that the offset 0 is base_addr_low but no size.
 */
typedef struct multiboot_memmap
{
    u32 size;
    u32 base_addr_low;
    u32 base_addr_high;
    u32 length_low;
    u32 length_high;
    u32 type;
}
multiboot_memmap_t;

/** Pointer to the multiboot information. */
extern multiboot_info_t *multiboot_info;

#endif /* !__ASSEMBLER__ */

/**
 * @}
 */

#endif /* __KERNEL_MULTIBOOT_H */

