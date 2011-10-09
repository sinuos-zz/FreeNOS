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

#include <FreeNOS/api.h>
#include <FreeNOS/io.h>
#include <FreeNOS/irq.h>
#include <FreeNOS/cpu.h>
#include <FreeNOS/types.h>
#include <FreeNOS/process.h>
#include <FreeNOS/printk.h>
#include <FreeNOS/error.h>
#include <FreeNOS/smp.h>

result_t api_execute(uint func,  ulong arg1,
		     ulong arg2, ulong arg3)
{
    process_t *p;

    switch (func)
    {
	case API_PROC_ID:
	    return proc_lookup(smp_cpu()->proc);

	case API_PROC_DESTROY:
	    if ((p = proc_find(arg1)) != NULL)
	    {
		return proc_destroy(p);
	    }
	    else
		return -ESRCH;

	case API_PROC_IDLE:
	    irq_enable();
	    for (;;) idle();

	case API_IO_READ:
	    return inb(arg1);

	case API_IO_WRITE:
	    outb(arg1, arg2);
	    return 0;

	default:
	    break;
    }
    return -EINVAL;
}
