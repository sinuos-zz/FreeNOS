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

#include <FreeNOS/gdt.h>
#include <FreeNOS/tss.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/virt_memory.h>

static gdt_t     ALIGN(PAGE_SIZE) gdt[SMP_MAX];
static gdt_ptr_t ALIGN(PAGE_SIZE) gdt_ptr[SMP_MAX];

void gdt_init(void)
{
    gdt_ptr_t *ptr = &gdt_ptr[smp_id()];

    /* Setup GDT entries. */
    gdt_set_entry(0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xffffffff, GDT_TYPE_CODE0);
    gdt_set_entry(2, 0, 0xffffffff, GDT_TYPE_DATA0);
    gdt_set_entry(3, 0, 0xffffffff, GDT_TYPE_CODE3);
    gdt_set_entry(4, 0, 0xffffffff, GDT_TYPE_DATA3);
    gdt_set_entry(5, (addr_t)tss_get(),
		      sizeof(tss_t), GDT_TYPE_TSS0);

    /* Set GDT pointer. */
    ptr->base  = (addr_t) &gdt[smp_id()];
    ptr->limit = sizeof(gdt_t);

    /* Load the GDT. */
    gdt_load(&gdt_ptr[smp_id()]);
}

void gdt_set_entry(uint index, addr_t base, uint limit, uint type)
{
    gdt_entry_t *g = &(gdt[smp_id()].entries[index]);

    g->limit_low  =  (limit / 4096) & 0xffff;
    g->limit_high = ((limit / 4096) >> 16) & 0xf;
    g->base_low   =   base & 0xffff;
    g->base_high1 =  (base >> 16) & 0xff;
    g->base_high2 =  (base >> 24) & 0xff;
    g->type       =   type;

    if (type != 0)
	g->flags  =   GDT_GRAN_4K | GDT_MODE_32;
}
