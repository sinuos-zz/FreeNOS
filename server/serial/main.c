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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "i8250.h"

result_t serial_write(char *buffer, size_t count)
{
    size_t bytes = 0;

    /* Write as much bytes as possible. */
    while (api_io_read(UART0 + LINESTATUS, sizeof(u8)) & TXREADY
	&& bytes < count)
    {
	api_io_write(UART0, buffer[bytes++], sizeof(u8));
    }
    return bytes;
}

int main(void)
{
    char buf[1024];

    /* Write a debug message. */
    snprintf(buf, sizeof(buf), "serial: running as PID %u\r\n",
	     getpid());
    serial_write(buf, strlen(buf));

    /* Let kill init. */
    api_proc_destroy(0);

    /* Don't die but also don't busy loop. */
    api_proc_idle();
    return 0;
}
