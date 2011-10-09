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

#ifndef __GDT_H
#define __GDT_H

#include "types.h"
#include "macro.h"

#define GDT_TYPE_CODE0 0x9a
#define GDT_TYPE_CODE3 0xfa
#define GDT_TYPE_DATA0 0x92
#define GDT_TYPE_DATA3 0xf2
#define GDT_TYPE_TSS0  0x89

#define GDT_MODE_32 (1 << 2)
#define GDT_GRAN_4K (1 << 3)

typedef struct gdt_entry
{
    u16 limit_low;
    u16 base_low;
    u8  base_high1;
    u8  type;
    u8  limit_high:4;
    u8  flags:4;
    u8  base_high2;
}
PACKED gdt_entry_t;

typedef struct gdt
{
    gdt_entry_t entries[6];
}
PACKED gdt_t;

typedef struct gdt_ptr
{
    u16 limit;
    u32 base;
}
PACKED gdt_ptr_t;

extern void gdt_init(void);
extern void gdt_load(gdt_ptr_t *ptr);
extern void gdt_set_entry(uint index, addr_t base, uint limit, uint type);

#endif /* __GDT_H */
