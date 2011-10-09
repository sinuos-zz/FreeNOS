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

#ifndef __LIBEXEC_H
#define __LIBEXEC_H

#include <sys/types.h>

/**
 * @defgroup libexec libexec
 * @{
 */

/** Executable format parsed successfully. */
#define LIBEXEC_FORMAT_OK	(1 << 0)

/** Executable format was detected, but contains errors. */
#define LIBEXEC_FORMAT_ERROR	(1 << 1)

/** Executable format not recognized by the parser. */
#define LIBEXEC_FORMAT_UNKNOWN	(1 << 2)

/** Maximum number of supported formats. */
#define LIBEXEC_MAX_FORMATS 16

/** Readable page. */
#define LIBEXEC_PAGE_R	(1 << 0)

/** Writable page. */
#define LIBEXEC_PAGE_W	(1 << 1)

/** Executable page. */
#define LIBEXEC_PAGE_X	(1 << 2)

/**
 * Abstracts a memory region read from a format.
 */
typedef struct memory_region
{
    /** Beginning of the region. */
    unsigned int base;

    /** Size of the memory region. */
    size_t size;

    /** Page protection flags. */
    int flags;

    /** Memory contents. */
    unsigned char *data;
}
memory_region_t;

/**
 * Abstracts a runnable program.
 */
typedef struct program
{
    /** Memory regions a program needs at runtime. */
    memory_region_t *regions;

    /** Number of memory regions. */
    size_t num_regions;

    /** The program entry point. */
    unsigned int entry;
}
program_t;

typedef int exec_format_t(FILE *fp, program_t **program);

extern int libexec_init(void);
extern int libexec_deinit(void);

extern int libexec_add_format(exec_format_t handler);

extern program_t * libexec_read(char *path);
extern char * libexec_error(void);
extern void libexec_set_error(char *format, ...);
extern int libexec_free(program_t *prog);

/**
 * @}
 */

#endif /* __LIBEXEC_H */

