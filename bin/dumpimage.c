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
#include <unistd.h>
#include <sys/stat.h>
#include <libboot.h>

static void dump_image(char *prog, char *path, boot_image_t *image)
{
    boot_prog_t *p;
    boot_seg_t *s;

    /* Parse boot image. */
    if (libboot_parse(image) != 0)
    {
	printf("%s: failed to libboot_parse() `%s': %s\r\n",
		prog, path, strerror(errno));
	return;
    }
    /* Dump boot_image_t header information. */
    printf("Revision: %u\r\n"
	   "Checksum: 0x%x\r\n"
	   "Variables Table Offset: 0x%x\r\n"
	   "Variables Table Count:  %u\r\n"
	   "Programs Table Offset:  0x%x\r\n"
	   "Programs Table Count:   %u\r\n"
	   "Segments Table Offset:  0x%x\r\n"
	   "Segments Table Count:   %u\r\n\r\n",
	    image->revision, image->checksum,
	    image->var_table_off,  image->var_table_count,
	    image->prog_table_off, image->prog_table_count,
	    image->seg_table_off,  image->seg_table_count);

    /* Loop boot programs in the image. */
    while ((p = libboot_getprog()) != NULL)
    {
	printf("Cmdline: %s\r\n"
	       "Flags:   0x%x\r\n"
	       "Entry:   0x%x\r\n"
	       "Segments Index: %u\r\n"
	       "Segments Count: %u\r\n"
	       "Stack Segment Index: %u\r\n\r\n",
	        p->path, p->flags, p->entry,
	        p->segments_index, p->segments_count,
	        p->stack_index);

	/* Loop segments of this boot program. */
	while ((s = libboot_getseg()) != NULL)
	{
	    printf("Address:     0x%x\r\n"
		   "Size:        %u bytes\r\n"
		   "Data Offset: 0x%x\r\n\r\n",
		    s->virt_base, s->size, s->offset);
	}
    }
}

int main(int argc, char **argv)
{
    struct stat st;
    void *image;
    FILE *fp;

    /* Verify command line arguments. */
    if (argc < 2)
    {
	printf("usage: %s IMAGE\r\n", argv[0]);
	return EXIT_FAILURE;
    }
    /* Stat the image file. */
    if (stat(argv[1], &st) != 0)
    {
	printf("%s: failed to stat() `%s': %s\r\n",
		argv[0], argv[1], strerror(errno));
	return EXIT_FAILURE;
    }
    /* Allocate memory block. */
    if (!(image = malloc(st.st_size)))
    {
	printf("%s: failed to malloc() image: %s\r\n",
		argv[0], strerror(errno));
	return EXIT_FAILURE;
    }
    /* Open image file. */
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
	printf("%s: failed to fopen() `%s': %s\r\n",
		argv[0], argv[1], strerror(errno));
	return EXIT_FAILURE;
    }
    /* Read image contents into memory. */
    if (fread(image, st.st_size, 1, fp) != 1)
    {
	printf("%s: failed to fread() `%s': %s\r\n",
		argv[0], argv[1], strerror(errno));
	fclose(fp);
	return EXIT_FAILURE;
    }
    /* Dump boot image information. */
    dump_image(argv[0], argv[1], image);

    /* Release resources. */
    fclose(fp);
    free(image);

    /* Success. */
    return EXIT_SUCCESS;
}
