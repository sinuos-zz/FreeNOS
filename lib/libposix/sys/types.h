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

#ifndef __LIBPOSIX_TYPES_H
#define __LIBPOSIX_TYPES_H

#include <FreeNOS/types.h>

/**
 * @defgroup libposix libposix (POSIX.1-2008)
 * @{
 */

/** Used for file block counts. */
typedef u32 blkcnt_t;

/** Used for block sizes. */
typedef u32 blksize_t;

/**
 * Used for system times in clock ticks or CLOCKS_PER_SEC.
 * @see <time.h>.
 */
typedef u32 clock_t;

/** Used for clock ID type in the clock and timer functions. */
typedef u32 clockid_t;

/** Used for device IDs. */
typedef u32 dev_t;

/** Used for group IDs. */
typedef u16 gid_t;

/**
 * Used as a general identifier.
 * Can be used to contain at least a pid_t, uid_t, or gid_t.
 */
typedef u16 id_t;

/** Used for file serial numbers. */
typedef u32 ino_t;

/** Used for some file attributes. */
typedef u32 mode_t;

/** Used for link counts. */
typedef u32 nlink_t;

/** Used for file sizes. */
typedef u32 off_t;

/** Used for a count of bytes or an error indication. */
typedef s32 ssize_t;

/** Used for time in seconds. */
typedef u32 time_t;

/** Used for timer ID returned by timer_create(). */
typedef id_t timer_t;

/** Used for user IDs. */
typedef u16 uid_t;

/**
 * @}
 */

#endif /* __LIBPOSIX_TYPES_H */

