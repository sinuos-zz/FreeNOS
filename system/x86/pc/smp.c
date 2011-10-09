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

#include <FreeNOS/multiboot.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/apic.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/phys_memory.h>
#include <FreeNOS/virt_memory.h>
#include <FreeNOS/types.h>
#include <FreeNOS/arch.h>
#include <string.h>

cpu_t cpus[SMP_MAX];
uint  cpus_total = 0;

static mp_entry_t * smp_parse_entry(mp_entry_t *entry)
{
    if (entry->type == MP_ENTRY_PROC)
    {
	cpus[cpus_total].id    = entry->apic_id;
	cpus[cpus_total].flags = SMP_AVAIL;

	if (entry->apic_id != apic_read(APIC_REG_ID))
	{
	    cpus[cpus_total].stack = palloc(STACK_SIZE);
	}
	else
	    cpus[cpus_total].stack = stackptr - STACK_SIZE + 1;

	cpus_total++;
	return entry + 1;
    }
    else
	return (mp_entry_t *) (((addr_t)(entry)) + 8);
}

void smp_init(void)
{
    mp_config_t *mpc;
    mp_entry_t *entry;
    size_t i;

    /** Initialize cpu_t array. */
    for (i = 0; i < SMP_MAX; i++)
    {
	cpus[0].id    = 0;
	cpus[0].flags = SMP_EMPTY;
	cpus[0].stack = 0;
	cpus[0].proc  = NULL;
	cpus[0].idle  = NULL;
    }
    /* Setup Application Processor entry code. */
    memcpy((void *) MP_BOOTENTRY_ADDR, smp_entry, PAGE_SIZE);

    /* Look for the Multiprocessor configuration. */
    if ( (mpc = smp_scan(0x0, 1024)) != 0 ||
         (mpc = smp_scan(arch_total_memory() - 1024, 1024)) != 0 ||
         (mpc = smp_scan(0xF0000, 0x10000)) != 0)
    {
	/* process config. */
	printk("MP header at %x\r\n", mpc);
	printk("Local APIC at %x\r\n", mpc->apic_addr);

	entry = (mp_entry_t *)(mpc+1);

	for (i = 0; i < mpc->count; i++)
	{
	    entry = smp_parse_entry(entry);
	}
    }
    else
    {
        cpus_total = 1;
        cpus[0].flags = SMP_AVAIL|SMP_ACTIVE;
    }
}

void smp_boot_all(void)
{
    uint i;

    for (i = 0; i < cpus_total; i++)
    {
	if (cpus[i].id != smp_id())
	{
	    smp_boot(&cpus[i]);
	}
    }
}

mp_config_t * smp_scan(addr_t addr, size_t count)
{
    size_t i;
    mp_float_t *mpf;

    for (i = 0; i < count; i += sizeof(addr_t))
    {
	mpf = (mp_float_t *)(addr + i);

	if (mpf->signature == MP_FLOAT_SIGNATURE)
	{
	    return (mp_config_t *) mpf->config_addr;
	}
    }
    return 0;
}

void smp_boot(cpu_t *cpu)
{
    ulong cfg;

    /* Set correct stackptr for loader() */
    stackptr = cpu->stack + STACK_SIZE - 1;

    /* Write APIC Destination. */
    cfg  = apic_read(APIC_REG_ICR2);
    cfg &= 0x00ffffff;
    apic_write(APIC_REG_ICR2, cfg | APIC_DEST(cpu->id));

    /* Assert INIT. */
    cfg  = apic_read(APIC_REG_ICR1);
    cfg &= ~0xcdfff;
    cfg |= (APIC_DEST_FIELD | APIC_DEST_LEVELTRIG |
            APIC_DEST_ASSERT | APIC_DEST_DM_INIT);
    apic_write(APIC_REG_ICR1, cfg);

    /* Write APIC Destination. */
    cfg  = apic_read(APIC_REG_ICR2);
    cfg &= 0x00ffffff;
    apic_write(APIC_REG_ICR2, cfg | APIC_DEST(cpu->id));

    /* Assert STARTUP. */
    cfg  = apic_read(APIC_REG_ICR1);
    cfg &= ~0xcdfff;
    cfg |= (APIC_DEST_FIELD | APIC_DEST_DM_STARTUP |
           (MP_BOOTENTRY_ADDR >> 12));
    apic_write(APIC_REG_ICR1, cfg);

    /* Now we have to wait for the CPU to be booted! */
    while (!(cpu->flags & SMP_ACTIVE));
}

uint smp_id(void)
{
    return (apic_read(APIC_REG_ID) >> 24);
}

uint smp_is_bootcpu(void)
{
    return smp_id() == 0;
}

cpu_t * smp_cpu(void)
{
    return &cpus[smp_id()];
}

