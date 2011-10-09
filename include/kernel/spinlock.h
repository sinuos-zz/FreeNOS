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

#ifndef __SPINLOCK_H
#define __SPINLOCK_H

#include "types.h"
#include "macro.h"

#define SPINLOCK_LOCKED   1
#define SPINLOCK_UNLOCKED 0

typedef struct spinlock
{
    u32 state;
}
PACKED spinlock_t;

extern void spinlock_init(spinlock_t *spinlock);
extern void spinlock_enter(spinlock_t *spinlock);
extern void spinlock_leave(spinlock_t *spinlock);

#endif /* __SPINLOCK_H */
