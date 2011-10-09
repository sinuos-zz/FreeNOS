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

#include <FreeNOS/printk.h>
#include <FreeNOS/io.h>
#include <FreeNOS/spinlock.h>
#include <FreeNOS/types.h>
#include <stdio.h>
#include <stdarg.h>

spinlock_t print_lock;

result_t serial_write(char *buffer, size_t count)
{
    size_t bytes = 0;

    /* Write as much bytes as possible. */
    while (inb(UART0 + LINESTATUS) & TXREADY && bytes < count)
    {
	outb(UART0, buffer[bytes++]);
    }
    return bytes;
}

void printk(const char *format, ...)
{
    char buf[1024];
    va_list args;
    size_t size, written = 0;
    result_t e;

    spinlock_enter(&print_lock);

    /* Write formatted string. */
    va_start(args, format);
    size = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    /* Write it to the first serial port. */
    while (written < size)
    {
	e = serial_write(buf + written, size - written);

	switch (e)
	{
	    /* End of file reached. */
	    case 0:
	        spinlock_leave(&print_lock);
		return;

	    /* Process bytes. */
	    default:
		written += e;
	}
    }
    spinlock_leave(&print_lock);
}
