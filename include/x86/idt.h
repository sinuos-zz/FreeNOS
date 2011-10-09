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

#ifndef __IDT_H
#define __IDT_H

#include "types.h"
#include "macro.h"

#define IDT_NUM	256
#define IDT_TRAP_GATE 0x8f
#define IDT_INT_GATE  0x8e
#define IDT_SYS_GATE  0xee

typedef struct idt_entry
{
    u16 offset_low;
    u16 segment;
    u8  reserved;
    u8  flags;
    u16 offset_high;
}
PACKED idt_entry_t;

typedef struct idt_ptr
{
    u16 limit;
    u32 base;
}
PACKED idt_ptr_t;

extern void idt_init(void);
extern void idt_load(void);
extern void idt_set_entry(u16 vector, u8 flags, addr_t isr);

#endif /* __IDT_H */

