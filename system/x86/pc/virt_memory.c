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

#include <FreeNOS/virt_memory.h>
#include <FreeNOS/phys_memory.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/apic.h>
#include <FreeNOS/error.h>
#include <string.h>

static void virt_dir_setmap(page_entry_t *dir, addr_t vaddr,
			    addr_t paddr, u16 flags)
{
    dir[DIRENTRY(vaddr)]  = paddr;
    dir[DIRENTRY(vaddr)] |= flags;
}

static void virt_tab_setmap(page_entry_t *tab, addr_t vaddr,
			    addr_t paddr, u16 flags)
{
    tab[TABENTRY(vaddr)]  = paddr;
    tab[TABENTRY(vaddr)] |= flags;
}

static void virt_ident_map(page_entry_t *dir, addr_t addr, size_t size)
{
    page_entry_t *tab;
    addr_t i;

    addr &= PAGE_MASK;

    /* Perform identity mapping. */
    for (i = addr; i < addr + size; i += PAGE_SIZE)
    {
    	/* Obtain pointer to page table. */
        if (dir[DIRENTRY(i)])
	    tab = (page_entry_t *) (dir[DIRENTRY(i)] & PAGE_MASK);
	else
	{
	    tab = (page_entry_t *) palloc(TAB_SIZE);
	    memset(tab, 0, TAB_SIZE);
	    virt_dir_setmap(dir, i, (addr_t) tab, PAGE_PR|PAGE_WR|PAGE_USER);
	}
	/* Map the page. */
	virt_tab_setmap(tab, i, addr + (i - addr), PAGE_PR|PAGE_WR);
    }
}

void virt_memory_init(void)
{
    page_entry_t *dir = (page_entry_t *) palloc(DIR_SIZE);

    /* Clear directory. */
    memset(dir, 0, DIR_SIZE);

    /* Map kernel, APIC, initial stack and page directory. */
    virt_ident_map(dir, KERN_VADDR, kernel_size);
    virt_ident_map(dir, APIC_BASE, PAGE_SIZE);
    virt_ident_map(dir, smp_cpu()->stack, STACK_SIZE);
    virt_dir_setmap(dir, DIR_VADDR, (addr_t) dir, PAGE_PR|PAGE_WR);

    /* Enable paging. */
    set_cr3(dir);
    add_cr0(CR0_PG);
}

result_t virt_map(addr_t dir_base, addr_t vaddr,
		  addr_t paddr, size_t size, u16 flags)
{
    page_entry_t *dir = TABADDR(dir_base, DIR_VADDR);
    page_entry_t *tab = 0;
    addr_t i, new_tab;

    /* Page align both addresses. */
    paddr &= PAGE_MASK;
    vaddr &= PAGE_MASK;

    /* Loop the virtual address range to map. */
    for (i = vaddr; i < vaddr + size; i += PAGE_SIZE)
    {
	tab = TABADDR(dir_base, i);

	if (!dir[DIRENTRY(i)])
	{
	    if (!(new_tab = palloc(TAB_SIZE)))
	    {
		return ENOMEM;
	    }
	    virt_dir_setmap(dir, i, new_tab, PAGE_PR|PAGE_WR|PAGE_USER);
	    virt_reload_addr(tab);
	    memset(tab, 0, TAB_SIZE);
	}
	/* Map the page. */
	virt_tab_setmap(tab, i, paddr + (i - vaddr), flags);
	virt_reload_addr(i);
    }
    return ESUCCESS;
}

void * virt_temp_map(addr_t addr, size_t size)
{
    addr_t vaddr = TMP_VADDR;
    page_entry_t *dir, *tab;

    /* Look for a free virtual memory range in the temp. table. */
    dir = LOCAL_DIR;
    tab = TABADDR(DIR_VADDR, vaddr);

    /* Find a free virtual address. */
    while (dir[DIRENTRY(vaddr)] & PAGE_PR &&
           tab[TABENTRY(vaddr)] & PAGE_PR)
    {
        /* Look for the next page in line. */
        vaddr += PAGE_SIZE;
        tab    = TABADDR(DIR_VADDR, vaddr);
    }
    /* Perform mapping. */
    virt_map(DIR_VADDR, vaddr, addr,
	     size, PAGE_PR|PAGE_WR);

    /* Success. */
    return (void *) vaddr;
}

void virt_unmap(void * vaddr, size_t size)
{
    virt_map(DIR_VADDR, (addr_t) vaddr, 0, size, 0);
}

void virt_release(addr_t dir_base)
{
    page_entry_t *dir = TABADDR(dir_base, DIR_VADDR);
    page_entry_t *tab;
    size_t i,j;

    for (i = 0; i < 1024; i++)
    {
	if (dir[DIRENTRY(i)] & PAGE_PR &&
	  !(dir[DIRENTRY(i)] & PAGE_PIN))
	{
	    tab = TABADDR(dir_base, i);

	    for (j = 0; i < 1024; j++)
	    {
		if (tab[j] & PAGE_PR &&
		  !(tab[j] & PAGE_PIN))
		{
		    pfree(tab[j] & PAGE_MASK);
		}
	    }
	}
    }
}

page_entry_t * virt_map_remote_dir(process_t *proc)
{
    virt_dir_setmap(LOCAL_DIR, RDIR_VADDR, proc->page_dir, PAGE_PR|PAGE_WR);
    virt_reload_all();
    return REMOTE_DIR;
}

void virt_unmap_remote_dir(process_t *proc)
{
    virt_dir_setmap(LOCAL_DIR, RDIR_VADDR, 0, 0);
    virt_reload_all();
}

void virt_memory_proc(process_t *proc, addr_t kern_stack, addr_t user_stack)
{
    page_entry_t *tmp_dir;

    /* Make a temporary mapping of the empty directory. */
    tmp_dir = virt_temp_map(proc->page_dir, PAGE_SIZE);

    /* Clear directory. Make mapping for itself. */
    memset(tmp_dir, 0, DIR_SIZE);
    virt_dir_setmap(tmp_dir, DIR_VADDR, proc->page_dir, PAGE_PR|PAGE_WR);

    /* Map directory. */
    virt_unmap(tmp_dir, PAGE_SIZE);
    virt_map_remote_dir(proc);

    /* Map kernel, APIC, initial stack and page directory. */
    virt_map(RDIR_VADDR, KERN_VADDR, KERN_VADDR,
	     kernel_size, PAGE_PR|PAGE_WR|PAGE_PIN);
    virt_map(RDIR_VADDR, APIC_BASE, APIC_BASE,
	     PAGE_SIZE, PAGE_PR|PAGE_WR|PAGE_PIN);
    virt_map(RDIR_VADDR, KERN_STACK, kern_stack,
	     STACK_SIZE, PAGE_PR|PAGE_WR);
    virt_map(RDIR_VADDR, USER_STACK, user_stack,
	     STACK_SIZE, PAGE_PR|PAGE_WR|PAGE_USER);

    /* Done. */
    virt_unmap_remote_dir(proc);
}
