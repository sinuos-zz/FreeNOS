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

#ifndef __TSS_H
#define __TSS_H

#include "types.h"
#include "macro.h"

typedef struct tss
{
    u16 prev_link, prev_link_high;
    u32 esp0;
    u16 ss0, ss0_high;
    u32 esp1;
    u16 ss1, ss1_high;
    u32 esp2;
    u16 ss2, ss2_high;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax, ecx, edx, ebx;
    u32 esp;
    u32 ebp;
    u32 esi, edi;
    u16 es, es_high;
    u16 cs, cs_high;
    u16 ss, ss_high;
    u16 ds, ds_high;
    u16 fs, fs_high;
    u16 gs, gs_high;
    u16 ldt, ldt_high;
    u16 reserved;
    u16 io_map_base;
}
PACKED tss_t;

extern void tss_init(void);
extern tss_t * tss_get(void);
extern void tss_update(addr_t kern_stack, uint kern_seg);

#endif /* __TSS_H */
