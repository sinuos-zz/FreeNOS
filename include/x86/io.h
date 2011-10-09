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

#ifndef __X86_IO_H
#define __X86_IO_H
#ifndef __ASSEMBLER__

/**
 * @defgroup x86kernel kernel (x86)
 * @{
 */

/**
 * Read a byte from a port.
 * @param port The I/O port to read from.
 * @return A byte read from the port.
 */
#define inb(port) \
({ \
    unsigned char b; \
    __asm__ volatile ("inb %%dx, %%al" : "=a" (b) : "d" (port)); \
    b; \
})

/**
 * Read a word from a port.
 * @param port The I/O port to read from.
 * @return Word read from the port.
 */
#define inw(port) \
({ \
    unsigned short w; \
    __asm__ volatile ("inw %%dx, %%ax" : "=a" (w) : "d" (port)); \
    w; \
})

/**
 * Output a byte to a port.
 * @param port Port to write to.
 * @param byte The byte to output.
 */
#define outb(port,byte) \
    __asm__ volatile ("outb %%al,%%dx"::"a" (byte),"d" (port))

/**
 * Output a word to a port.
 * @param port Port to write to.
 * @param byte The word to output.
 */
#define outw(port,word) \
    __asm__ volatile ("outw %%ax,%%dx"::"a" (word),"d" (port))

/**
 * Output a long to a I/O port.
 * @param port Target I/O port.
 * @param l The long 32-bit number to output.
 */
#define outl(port,l) \
    __asm__ volatile ("outl %%eax,%%dx"::"a" (l),"d" (port))

#endif /* __ASSEMBLER__ */
#endif /* __X86_IO_H */

