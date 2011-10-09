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

#include <FreeNOS/cpu.h>
#include <FreeNOS/phys_memory.h>
#include <FreeNOS/virt_memory.h>
#include <FreeNOS/spinlock.h>
#include <FreeNOS/types.h>
#include <FreeNOS/arch.h>
#include <FreeNOS/printk.h>
#include <string.h>

/* Private variables. */
static spinlock_t phys_memory_lock;
static size_t memory_size, memory_avail;
static u8 *memory_map, *memory_map_end;

/* Defined in the linker script. */
extern addr_t _kernel_start,   _kernel_end,
	      _boot_img_start, _boot_img_end;

/* Begin and end addresses of the kernel and bootimg. */
addr_t kernel_start   = (addr_t) &_kernel_start;
addr_t kernel_end     = (addr_t) &_kernel_end;
addr_t boot_img_start = (addr_t) &_boot_img_start;
addr_t boot_img_end   = (addr_t) &_boot_img_end;
size_t kernel_size    = 0;

static int paddr_is_marked(addr_t paddr)
{
    size_t index = (paddr >> PAGE_SHIFT) / 8;
    size_t bit   = (paddr >> PAGE_SHIFT) % 8;

    return memory_map[index] & (1 << bit);
}

static void paddr_set_mark(addr_t paddr, int marked)
{
    size_t index = (paddr >> PAGE_SHIFT) / 8;
    size_t bit   = (paddr >> PAGE_SHIFT) % 8;

    if (marked)
    {
	memory_map[index] |=  (1 << bit);
	memory_avail -= PAGE_SIZE;
    }
    else
    {
	memory_map[index] &= ~(1 << bit);
	memory_avail += PAGE_SIZE;
    }
}

void phys_memory_init(void)
{
    /* Initialize lock. */
    spinlock_init(&phys_memory_lock);

    /* Initialize (local) variables. */
    memory_size  = arch_total_memory();
    memory_avail = memory_size;

    /* Allocate physical memory bitmap. */
    memory_map     = (u8 *)(kernel_end);
    memory_map_end = memory_map + (memory_size / PAGE_SIZE / 8);

    /* Set kernel size. */
    kernel_size  = (kernel_end - kernel_start) +
		   (memory_map_end - memory_map);

    /* Clear memory map. */
    memset(memory_map, 0, memory_size / PAGE_SIZE / 8);

    /* Mark BIOS and kernel memory used. */
    palloc_from(0, 1024*1024);
    palloc_from(kernel_start, kernel_size);

    /* Output system memory. */
    printk("Memory: %dKB total, %dKB avail, %dKB kernel, %dKB bootimg\r\n",
	    memory_size / 1024, memory_avail / 1024,
	    (kernel_size-(boot_img_end-boot_img_start)) / 1024,
	    (boot_img_end-boot_img_start) / 1024);
}

addr_t palloc(size_t sz)
{
    return palloc_from(0, sz);
}

addr_t palloc_from(addr_t base, size_t sz)
{
    addr_t from = 0, count = 0, i, j, ret = 0;

    /* Don't allow two or more CPU's in here. */
    spinlock_enter(&phys_memory_lock);

    /* Loop the memoryMap for a free block. */
    for (i = base & PAGE_MASK; i < memory_size; i += PAGE_SIZE)
    {
	if (!paddr_is_marked(i))
	{
	    /* Remember this page. */
	    if (!count)
	    {
		from  = i;
		count = 1;
	    }
	    else
		count++;

	    /* Are there enough contigious pages? */
	    if (count * PAGE_SIZE >= sz)
	    {
		for (j = from; j < from + (count * PAGE_SIZE); j += PAGE_SIZE)
		{
		    paddr_set_mark(j, 1);
		}
		ret = from;
		break;
	    }
	}
	else
	{
	    from = count = 0;
	}
    }
    /* Release lock and return the result to the invokee. */
    spinlock_leave(&phys_memory_lock);
    return ret;
}

void pfree(addr_t paddr)
{
    spinlock_enter(&phys_memory_lock);
    paddr_set_mark(paddr & PAGE_MASK, 0);
    spinlock_leave(&phys_memory_lock);
}

