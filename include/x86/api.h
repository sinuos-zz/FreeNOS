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

#ifndef __X86_API_H
#define __X86_API_H

#include "types.h"

/**
 * Perform a kernel trap with no arguments.
 * @param num Unique number of the handler to execute.
 * @return An integer.
 */
__inline__ static ulong sys_call0(ulong num)
{
    ulong ret;
    __asm__ volatile ("int $0x90" : "=a"(ret) : "a"(num));
    return ret;
}

/**
 * Perform a kernel trap with 1 argument.
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @return An integer.
 */
__inline__ static ulong sys_call1(ulong num, ulong arg1)
{
    ulong ret;
    __asm__ volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1));
    return ret;
}

/**
 * Perform a kernel trap with 2 arguments.
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @return An integer.
 */
__inline__ static ulong sys_call2(ulong num, ulong arg1, ulong arg2)
{
    ulong ret;
    __asm__ volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1),
					        "b"(arg2));
    return ret;
}

/**
 * Perform a kernel trap with 3 arguments.
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @param arg3 Third argument becomes EDX.
 * @return An integer..
 */
__inline__ static ulong sys_call3(ulong num, ulong arg1,
				  ulong arg2, ulong arg3)
{
    ulong ret;
    __asm__ volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1),
						"b"(arg2), "d"(arg3));
    return ret;
}

#include <kernel/api.h>

#endif /* __X86_API_H */
