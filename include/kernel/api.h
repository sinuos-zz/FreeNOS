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

#ifndef __API_H
#define __API_H

#include "types.h"

#define API_PROC_ID      (1 << 0)
#define API_PROC_DESTROY (1 << 1)
#define API_PROC_IDLE	 (1 << 2)
#define API_IO_READ      (1 << 3)
#define API_IO_WRITE     (1 << 4)

__inline__ static pid_t api_proc_id(void)
{
    return (pid_t) sys_call0(API_PROC_ID);
}

__inline__ static result_t api_proc_destroy(pid_t pid)
{
    return sys_call1(API_PROC_DESTROY, pid);
}

__inline__ static void api_proc_idle(void)
{
    sys_call0(API_PROC_IDLE);
}

__inline__ static result_t api_io_read(uint port, size_t size)
{
    return sys_call2(API_IO_READ, port, size);
}

__inline__ static result_t api_io_write(uint port, uint value, size_t size)
{
    return sys_call3(API_IO_WRITE, port, value, size);
}

extern result_t api_execute(uint func, ulong arg1,
			    ulong arg2, ulong arg3);

#endif /* __API_H */
