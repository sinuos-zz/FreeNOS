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
#include <FreeNOS/process.h>
#include <FreeNOS/types.h>
#include <FreeNOS/spinlock.h>
#include <FreeNOS/arch.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/irq.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/phys_memory.h>
#include <FreeNOS/virt_memory.h>
#include <libboot.h>
#include <string.h>

extern boot_image_t boot_img;
static process_t proc_table[PROC_MAX];
static spinlock_t proc_lock;

void proc_table_init(void)
{
    boot_prog_t *prog;
    boot_seg_t *seg;
    process_t *proc;
    pid_t pid;
    int num = 0;

    printk("Boot image at %x\r\n", boot_img_start);

    /* Initialize process table lock. */
    spinlock_init(&proc_lock);

    /* Clear table. */
    memset(proc_table, 0, sizeof(proc_table));

    /* Parse the boot image. */
    libboot_parse(&boot_img);

    /*
     * Loop the boot image.
     */
    while ((prog = libboot_getprog()))
    {
	printk("Loading `%s' ... ", prog->path);

	pid  = proc_create(prog->entry);
	proc = proc_find(pid);
	virt_map_remote_dir(proc);

	printk(" PID %u ... ", pid);

	/* Map program memory segments. */
	while ((seg = libboot_getseg()))
	{
	    printk("%x(%u) => %x ... ",
		     boot_img_start + seg->offset, seg->size, seg->virt_base);
	    virt_map(RDIR_VADDR, seg->virt_base,
		     boot_img_start + seg->offset, seg->size,
		     PAGE_PR|PAGE_WR|PAGE_USER);
	}
	virt_unmap_remote_dir(proc);
	printk("\r\n");

	/* Start a copy of idle program on each CPU. */
	if (prog->flags & BOOTPROG_ISIDLE)
	{
	    cpus[num].idle = proc;

	    if (num < cpus_total - 1)
	    {
		libboot_repeat_prog();
	        num++;
	    }
	    else
		num = 0;
	}
	else
	    proc_resume(proc);
    }
}

pid_t proc_create(addr_t entry)
{
    pid_t i;
    addr_t kern_stack, user_stack;

    spinlock_enter(&proc_lock);

    /* Find an empty process_t. */
    for (i = 0; i < PROC_MAX; i++)
    {
	if (!(proc_table[i].flags & PROC_AVAIL))
	{
	    /* Fill process table entry. */
	    proc_table[i].flags    = PROC_AVAIL;
	    proc_table[i].page_dir = (addr_t) palloc(DIR_SIZE);

	    /* Allocate stacks. */
	    kern_stack = palloc(STACK_SIZE);
	    user_stack = palloc(STACK_SIZE);

	    /* Initialize process. */
	    virt_memory_proc(&proc_table[i], kern_stack,
			     user_stack);
	    arch_init_proc(&proc_table[i], entry,
			    kern_stack, user_stack);

	    /* Done! */
	    spinlock_leave(&proc_lock);
	    return i;
	}
    }
    /* No more process_t slots available. */
    spinlock_leave(&proc_lock);
    return (pid_t) -1;
}

process_t * proc_find(pid_t pid)
{
    return &proc_table[pid];
}

pid_t proc_lookup(process_t *proc)
{
    return proc ? proc - proc_table : (pid_t) -1;
}

void proc_resume(process_t *p)
{
    spinlock_enter(&proc_lock);
    p->flags |= PROC_READY;
    spinlock_leave(&proc_lock);
}

void proc_pause(process_t *p)
{
    spinlock_enter(&proc_lock);
    p->flags &= ~PROC_READY;
    spinlock_leave(&proc_lock);
}

result_t proc_destroy(process_t *p)
{
    /*
     * Cannot kill itself. This avoid the
     * scenario where all pages have been pfree()'ed, including page_dir,
     * and some other CPU claims it and messes it up before we
     * are able to schedule anything yet. Another process simply needs to
     * do the dirty work while safely running under its own page_dir.
     */
    if (smp_cpu()->proc == p || p->flags & PROC_RUNNING)
    {
	return -EINVAL;
    }
    /* Release physical memory. */
    virt_map_remote_dir(p);
    virt_release(RDIR_VADDR);
    virt_unmap_remote_dir(p);

    /* Clear process table entry. */
    spinlock_enter(&proc_lock);
    memset(p, 0, sizeof(*p));
    spinlock_leave(&proc_lock);

    /* Done. */
    return 0;
}

void proc_schedule(void)
{
    static pid_t last_pid = 0;
    process_t *old  = smp_cpu()->proc;
    process_t *new;
    pid_t start = last_pid;

    /* No IRQ's allowed at this point. */
    irq_disable();

    /* Look for a new runnable process. */
    spinlock_enter(&proc_lock);
    while (1)
    {
	last_pid = (last_pid + 1) % PROC_MAX;

	/* Fall back to the current or idle process if nobody ready. */
	if (last_pid == start)
	{
	    if (old && old->flags & PROC_READY)
		new = old;
	    else
		new = smp_cpu()->idle;
	    break;
	}
	/*
	 * Try this process, if availble,
	 * ready and not already running.
	 */
	if (proc_table[last_pid].flags & PROC_AVAIL &&
	    proc_table[last_pid].flags & PROC_READY &&
	  !(proc_table[last_pid].flags & PROC_RUNNING))
	{
	    new = &proc_table[last_pid];
	    break;
	}
    }
    /* Mark the new process running. */
    new->flags |= PROC_RUNNING;
    smp_cpu()->proc = new;
    spinlock_leave(&proc_lock);

    /* Continue execution of the chosen process. */
    if (old != new)
    {
	arch_context_switch(old, new);
    }
}
