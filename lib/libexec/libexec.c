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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "libexec.h"
#include "elf.h"

static exec_format_t * formats[LIBEXEC_MAX_FORMATS];
static int num_formats = 0;
static char error_buffer[1024];

int libexec_init(void)
{
    /* Clear formats table. */
    memset(formats, 0, sizeof(formats));

    /* Register known builtin formats. */
    libexec_add_format(elf_read);
    return 0;
}

int libexec_deinit(void)
{
    return 0;
}

int libexec_add_format(exec_format_t handler)
{
    /* Only add format handler if enough space. */
    if (num_formats < LIBEXEC_MAX_FORMATS)
    {
	formats[num_formats++] = handler;
	return 0;
    }
    else
	return 1;
}

program_t * libexec_read(char *path)
{
    FILE *fp;
    program_t *prog;
    int i;

    /* Attempt to open the file. */
    if ((fp = fopen(path, "r")) == NULL)
    {
	libexec_set_error("failed to fopen() `%s': %s",
			   path, strerror(errno));
	return NULL;
    }
    /* Invoke each format handler in order. */
    for (i = 0; i < num_formats; i++)
    {
	fseek(fp, 0, SEEK_SET);

	/* Attempt to parse the format. */
	switch(formats[i](fp, &prog))
	{
	    case LIBEXEC_FORMAT_OK:
		fclose(fp);
	        return prog;

	    case LIBEXEC_FORMAT_ERROR:
		return NULL;

	    case LIBEXEC_FORMAT_UNKNOWN:
		continue;
	}
    }
    /* Done. */
    fclose(fp);

    /* Unknown format. */
    libexec_set_error("unknown format");
    return NULL;
}

char * libexec_error(void)
{
    return error_buffer;
}

void libexec_set_error(char *format, ...)
{
    va_list args;

    /* Write formatted error to static buffer. */
    va_start(args, format);
    vsnprintf(error_buffer, sizeof(error_buffer), format, args);
    va_end(args);
}

int libexec_free(program_t *prog)
{
    int i;

    /* Release all memory region data. */
    for (i = 0; i < prog->num_regions; i++)
    {
	free(prog->regions[i].data);
    }
    /* Release program_t. */
    free(prog->regions);
    free(prog);
    return 0;
}

