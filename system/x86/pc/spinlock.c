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
#include <FreeNOS/spinlock.h>
#include <FreeNOS/smp.h>
#include <FreeNOS/irq.h>

void spinlock_init(spinlock_t *spinlock)
{
    spinlock->state = SPINLOCK_UNLOCKED;
}

void spinlock_enter(spinlock_t *spinlock)
{
    __asm__ volatile ("1: movl %0, %%eax;"
		      "   lock;"
		      "   cmpxchgl %1,(%2);"
		      "   jne 1b"
		      :: "r" (SPINLOCK_UNLOCKED),
		         "r" (SPINLOCK_LOCKED),
		         "r" (&spinlock->state) : "eax");
}

void spinlock_leave(spinlock_t *spinlock)
{
    spinlock->state = SPINLOCK_UNLOCKED;
}
