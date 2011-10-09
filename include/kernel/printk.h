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

#ifndef __PRINTK_H
#define __PRINTK_H

#include "spinlock.h"

/** I/O port of the first serial UART. */
#define UART0      0x3f8

/** UART Status register. */
#define LINESTATUS 5

/** UART Status Ready Flag. */
#define TXREADY    0x20

/**
 * Print a debug message to the system console.
 *
 * @param format Formatted message to print.
 * @param ... Variable list of arguments for the format.
 */
extern void printk(const char *format, ...);

extern spinlock_t print_lock;

#endif /* __PRINTK_H */


