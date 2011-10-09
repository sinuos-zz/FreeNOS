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

#ifndef __MACRO_H
#define __MACRO_H

/** Binary 1 means true. */
#define TRUE		1

/** Binary zero means false. */
#define FALSE		0

/** NULL means zero. */
#ifndef NULL
#define NULL		(void *)0
#endif

/** Stringfies the given input. */
#define QUOTE(x) \
    #x

/** Indirect quoting. */
#define IQUOTE(x) \
    QUOTE(x)

/**
 * Calculate a division, and round to up any remaining.
 * @param number Input number.
 * @param divisor Divisor number.
 * @return Ceiled number.
 */
#define CEIL(number,divisor) \
     (number) % (divisor) ?  \
    ((number) / (divisor)) + 1 : (number) / (divisor)

/**
 * Can be used to link a symbol inside a specific section.
 * @param s Section name.
 */
#define SECTION(s) \
    __attribute__((__section__(s)))

/**
 * Declares an symbol to be forcibly "used".
 */
#define USED \
    __attribute__((__used__))

/**
 * Aligns a symbol at the given boundary.
 * @param n Boundary to align.
 */
#define ALIGN(n) \
    __attribute__((aligned(n)))

/**
 * Don't reserve extra space in data structures.
 */
#define PACKED \
    __attribute__((packed))

#endif /* __MACRO_H */

