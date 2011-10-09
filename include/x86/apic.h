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

#ifndef __APIC_H
#define __APIC_H

#include "types.h"

#define APIC_BASE		0xfee00000
#define APIC_REG_ID		0x20
#define APIC_REG_VERSION	0x30
#define APIC_REG_EOI		0xb0
#define APIC_REG_SIVR		0xf0
#define APIC_REG_ICR1		0x300
#define APIC_REG_ICR2		0x310
#define APIC_REG_TIMER		0x320
#define APIC_REG_LINT0		0x350
#define APIC_REG_LINT1		0x360
#define APIC_REG_DCR		0x3e0
#define APIC_REG_IC		0x380
#define APIC_REG_CC		0x390

#define APIC_DEST(x) ((x) << 24)
#define APIC_DEST_FIELD		0x00000
#define APIC_DEST_LEVELTRIG	0x08000
#define APIC_DEST_ASSERT        0x04000
#define APIC_DEST_DM_INIT       0x00500
#define APIC_DEST_DM_STARTUP	0x00600

#define APIC_TIMER_VECTOR	48
#define APIC_TIMER_DIV16	3
#define APIC_TIMER_PERIODIC	(1 << 17)

#define apic_read(reg) (*(ulong *)(APIC_BASE + (reg)))
#define apic_write(reg,value) ((*(ulong *)(APIC_BASE + (reg))) = (value))

#define apic_enable() \
    ({ \
	uint v; \
		\
        v  = apic_read(APIC_REG_SIVR); \
        v |= 0x100; \
        apic_write(APIC_REG_SIVR, v); \
    })

#endif /* __APIC_H */

