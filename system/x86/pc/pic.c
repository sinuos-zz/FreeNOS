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

#include <FreeNOS/pic.h>
#include <FreeNOS/io.h>

void pic_init(void)
{
    /* ICW1: Initialize PIC's (Edge triggered, Cascade) */
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    /* ICW2: Remap IRQ's to interrupts 32-47. */
    outb(PIC1_DATA, PIC_IRQ_BASE);
    outb(PIC2_DATA, PIC_IRQ_BASE + 8);

    /* ICW3: PIC2 is connected to PIC1 via IRQ2. */
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    /* ICW4: 8086 mode, fully nested, not buffered, no implicit EOI. */
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    /* OCW1: Disable all IRQ's for now. */
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);

    /* Make sure to enable PIC2. */
    pic_enable_irq(2);
}

void pic_enable_irq(u16 vector)
{
    if (vector < 8)
        outb(PIC1_DATA, inb(PIC1_DATA) & ~(1 << vector));
    else
        outb(PIC2_DATA, inb(PIC2_DATA) & ~(1 << (vector - 8)));
}

void pic_disable_irq(u16 vector)
{
    if (vector < 8)
        outb(PIC1_DATA, inb(PIC1_DATA) | (1 << vector));
    else
        outb(PIC2_DATA, inb(PIC2_DATA) | (1 << (vector - 8)));
}

void pic_ack_irq(u16 vector)
{
    /* End of Interrupt to slave. */
    if (vector >= 8)
    {
        outb(PIC2_CMD, PIC_EOI);
    }
    /* End of Interrupt to master. */
    outb(PIC1_CMD, PIC_EOI);
}

