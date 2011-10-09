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
#include <string.h>
#include <errno.h>
#include "libexec.h"
#include "elf.h"

/** Maximum number of ELF segments. */
#define MAX_SEGS 16

/**
 * Parse ELF regions.
 * @param fp Open file pointer to the ELF program to parse.
 * @param header ELF program header.
 * @param num_regions Number of regions counter.
 * @return Pointer to an array of memory_region_t's on success
 *         and NULL on failure.
 */
memory_region_t * elf_regions(FILE *fp, elf_header_t *header,
			      size_t *num_regions)
{
    elf_segment_t segments[MAX_SEGS];
    memory_region_t *regions;
    int i;

    /* Clear to zero. */
    (*num_regions) = 0;

    /* Must be of the same sizes. */
    if (!(header->prog_ent_size == sizeof(elf_segment_t) &&
          header->prog_ent_count < MAX_SEGS))
    {
	libexec_set_error(
		"invalid ELF program header entry size %d, should be %lu",
		 header->prog_ent_size, sizeof(elf_segment_t));
	return NULL;
    }
    /* Point to the program header. */
    if (fseek(fp, header->prog_header_off, SEEK_SET) == -1)
    {
	libexec_set_error("failed to fseek() ELF program header: %s",
			   strerror(errno));
	return NULL;
    }
    /* Read all segments. */
    if (fread(&segments, sizeof(elf_segment_t),
	       header->prog_ent_count, fp) != header->prog_ent_count)
    {
	libexec_set_error("failed to fread() %d elf_segment_t's: %s",
	    		   header->prog_ent_count, strerror(errno));
	return NULL;
    }
    /* Allocate memory regions. */
    if (!(regions = malloc(sizeof(memory_region_t) * MAX_SEGS)))
    {
	libexec_set_error("failed to malloc() memory_region_t's: %s",
			   strerror(errno));
	return NULL;
    }
    /* Fill in the memory regions. */
    for (i = 0; i < header->prog_ent_count; i++)
    {
	/* We are only interested in loadable segments. */
	if (segments[i].type != ELF_SEGMENT_LOAD)
	{
	    continue;
	}
	/* Fill the memory region. */
	regions[*num_regions].base  = segments[i].virt_base;
	regions[*num_regions].size  = segments[i].memory_size;
	regions[*num_regions].flags = LIBEXEC_PAGE_R|
				      LIBEXEC_PAGE_W|LIBEXEC_PAGE_X;

	/* Allocate memory for segment contents. */
	if (!(regions[*num_regions].data = malloc(segments[i].memory_size)))
	{
	    libexec_set_error("failed to malloc() memory_region_t's: %s",
			       strerror(errno));
	    free(regions); /* TODO: also release all region.data's please! */
	    return NULL;
	}
	/* Read segment contents from file. */
	if (fseek(fp, segments[i].offset, SEEK_SET) == -1 ||
	    fread(regions[*num_regions].data, segments[i].file_size, 1, fp) < 0)
	{
	    libexec_set_error("failed to fread() elf_segment_t content: %s",
			       strerror(errno));
	    return NULL;
	}
	/* Nulify remaining space. */
	if (segments[i].memory_size > segments[i].file_size)
	{
	    memset(regions[*num_regions].data + segments[i].file_size, 0,
		   segments[i].memory_size - segments[i].file_size);
	}
	/* Increment counter. */
	(*num_regions)++;
    }
    /* All done. */
    return regions;
}

int elf_read(FILE *fp, program_t **program)
{
    elf_header_t header;
    memory_region_t *regions;
    int fd;

    /* Read ELF header. */
    if (fread(&header, sizeof(header), 1, fp) != 1)
    {
	return LIBEXEC_FORMAT_UNKNOWN;
    }
    /* Verify ELF magic. */
    if (!(header.ident[ELF_INDEX_MAGIC0] == ELF_MAGIC0 &&
	  header.ident[ELF_INDEX_MAGIC1] == ELF_MAGIC1 &&
	  header.ident[ELF_INDEX_MAGIC2] == ELF_MAGIC2 &&
	  header.ident[ELF_INDEX_MAGIC3] == ELF_MAGIC3))
    {
	return LIBEXEC_FORMAT_UNKNOWN;
    }
    /* Only accept current, 32-bit ELF executable programs. */
    if (!(header.ident[ELF_INDEX_CLASS] == ELF_CLASS_32 &&
          header.version == ELF_VERSION_CURRENT &&
          header.type    == ELF_TYPE_EXEC))
    {
	return LIBEXEC_FORMAT_UNKNOWN;
    }
    /* Allocate program buffer. */
    if (!((*program) = (program_t *) malloc(sizeof(program_t))))
    {
	libexec_set_error("failed to malloc() program_t: %s",
			   strerror(errno));
	return LIBEXEC_FORMAT_ERROR;
    }
    /* Fetch memory regions. */
    if (!(regions = elf_regions(fp, &header, &((*program)->num_regions))))
    {
	free(*program);
	return LIBEXEC_FORMAT_ERROR;
    }
    /* Fill program_t */
    (*program)->regions = regions;
    (*program)->entry   = header.entry;

    /* All done. */
    return LIBEXEC_FORMAT_OK;
}

