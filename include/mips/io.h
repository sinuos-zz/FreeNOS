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

#ifndef __IO_H
#define __IO_H
#ifndef __ASSEMBLER__

/** MIPS systems have memory-mapped I/0. */
#define BASE 0xbfd00000

/**
 * @defgroup mipskernel kernel (mips)
 * @{
 */

/**
 * Read a byte from a port.
 * @param port The I/O port to read from.
 * @return A byte read from the port.
 */
#define inb(port) (*((unsigned char *) (BASE + (port))))

/**
 * Output a byte to a port.
 * @param port Port to write to.
 * @param byte The byte to output.
 */
#define outb(port,byte) ((*(unsigned char *) (BASE + (port))) = (byte))

/**
 * @}
 */

#endif /* __ASSEMBLER__ */
#endif /* __IO_H */

