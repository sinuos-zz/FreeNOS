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

#include <FreeNOS/smp.h>
#include <FreeNOS/irq.h>
#include <FreeNOS/arch.h>
#include <FreeNOS/phys_memory.h>
#include <FreeNOS/virt_memory.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/spinlock.h>
#include <FreeNOS/process.h>
#include <FreeNOS/types.h>
#include <FreeNOS/version.h>

int main(void)
{
    arch_init_cpu();

    if (smp_is_bootcpu())
    {
        spinlock_init(&print_lock);

        printk("FreeNOS %s [%s/%s] (%s@%s) (%s) %s\r\n",
                RELEASE, ARCH, SYSTEM, BUILDUSER,
                BUILDHOST, COMPILER, DATETIME);
	printk("%s\r\n\r\n", COPYRIGHT);

        phys_memory_init();
	irq_init();
        smp_init();
	virt_memory_init();
        proc_table_init();
        arch_init_clock();
	smp_boot_all();
    }
    else
	virt_memory_init();

    printk("CPU#%d: running, stack %x\r\n",
            smp_id(), smp_cpu()->stack);

    irq_prepare();
    arch_enable_clock();
    irq_enable();
    proc_schedule();

    return 0;
}
