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

#include <FreeNOS/tss.h>
#include <FreeNOS/gdt.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/virt_memory.h>
#include <string.h>

static tss_t ALIGN(PAGE_SIZE) tss[SMP_MAX];

void tss_init(void)
{
    memset(&tss[smp_id()], 0, sizeof(tss_t));
    ltr(KERN_TSS_SEL);
}

tss_t * tss_get(void)
{
    return &tss[smp_id()];
}

void tss_update(addr_t kern_stack, uint kern_seg)
{
    tss[smp_id()].esp0 = kern_stack;
    tss[smp_id()].ss0  = kern_seg;
}
