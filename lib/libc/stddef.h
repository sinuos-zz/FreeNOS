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

#ifndef __LIBC_STDDEF_H
#define __LIBC_STDDEF_H

#include <sys/types.h>

/**
 * @defgroup libc libc (ISO C99)
 * @{
 */

#ifndef NULL
/**
 * Null pointer constant.
 *
 * The macro shall expand to an integer constant expression
 * with the value 0 cast to type void *.
 */
#define NULL ((void *) 0)
#endif /* NULL */

#ifndef offsetof
/**
 * Calculate offsets in data structures.
 *
 * Integer constant expression of type size_t, the value
 * of which is the offset in bytes to the structure member
 * (member-designator), from the beginning of its structure (type).
 *
 * @param TYPE structure type.
 * @param MEMBER structure member field.
 * @return Offset of MEMBER inside TYPE in bytes as a size_t.
 */
#define offsetof(TYPE, MEMBER) \
    ((size_t) &((TYPE *)0)->MEMBER)
#endif /* offsetof */

/**
 * @}
 */

#endif /* __LIBC_STDDEF_H */

