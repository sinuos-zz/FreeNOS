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

#ifndef __IRQ_H
#define __IRQ_H

#include "types.h"
#include "cpu.h"

typedef void (*irq_handler_t)(cpu_state_t *state);

extern void irq_init(void);
extern void irq_prepare(void);
extern void irq_enable(void);
extern void irq_disable(void);
extern void irq_install(u16 irq, irq_handler_t handler);
extern void irq_ignore(cpu_state_t *state);

extern irq_handler_t irq_handler[MAX_IRQ];

#endif /* __IRQ_H */

