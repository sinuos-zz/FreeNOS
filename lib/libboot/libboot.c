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
#include <errno.h>
#include <string.h>
#include "libboot.h"

/**
 * @brief In-memory representation of a boot program.
 *
 * The libboot_addprog() and libboot_addseg() functions are
 * used to construct an in-memory boot program. Eventually the
 * libboot_write() function will use this structure to build
 * the final on-disk representation of the boot image.
 */
typedef struct boot_entry
{
    boot_prog_t prog; /**<< Boot program definition. */
    boot_seg_t *segs; /**<< Array of memory segment descriptions. */
    size_t num_segs;  /**<< Total number of memory segments. */
    void **data;      /**<< Array of memory segment data pointers. */
}
boot_entry_t;

static boot_entry_t *entries;
static int num, total_segs;
static boot_image_t *image;
static int cur_prog, cur_seg;
static char err_buf[128];


static void libboot_reset(void)
{
    int i,j;

    /* Free memory. */
    for (i = 0; i < num; i++)
    {
	for (j = 0; j < entries[i].num_segs; j++)
	{
	    free(entries[i].data[j]);
	}
        free(entries[i].segs);
	free(entries[i].data);
    }
    if (entries != NULL)
	free(entries);

    /* Clear local variables. */
    entries = NULL;
    num = 0;
    total_segs = 0;
    cur_prog = cur_seg = 0;
    image = NULL;
}

int libboot_init(void)
{
    entries = NULL;
    num = 0;
    total_segs = 0;
    return 0;
}

int libboot_deinit(void)
{
    libboot_reset();
    return 0;
}

int libboot_parse(boot_image_t *img)
{
    libboot_reset();
    image = img;
    return 0;
}

boot_prog_t * libboot_getprog(void)
{
    unsigned char *img = (unsigned char *) image;
    unsigned char *tab = (img + (image->prog_table_off));

    if (cur_prog < image->prog_table_count)
    {
	cur_seg = 0;
	return ((boot_prog_t *) (tab)) + cur_prog++;
    }
    return NULL;
}

void libboot_repeat_prog(void)
{
    if (cur_prog > 0)
    {
	cur_prog--;
    }
}

boot_seg_t * libboot_getseg(void)
{
    unsigned char *img  = (unsigned char *) image;
    unsigned char *ptab = (img + (image->prog_table_off));
    unsigned char *stab = (img + (image->seg_table_off));
    boot_prog_t *p = ((boot_prog_t *) ptab) + (cur_prog - 1);

    if (cur_seg < p->segments_count &&
        cur_seg + p->segments_count < image->seg_table_count)
    {
	return ((boot_seg_t *) (stab)) + (p->segments_index + cur_seg++);
    }
    return NULL;
}

boot_seg_t * libboot_getstack(void)
{
    return NULL;
}

int libboot_addprog(char *cmd, unsigned int entry,
			       unsigned short flags)
{
    /* Enlarge boot_entry_t array by one. */
    if (!(entries = realloc(entries, sizeof(boot_entry_t) *
				    (++num))))
    {
        snprintf(err_buf, sizeof(err_buf),
                "failed to realloc() boot_entry_t: %s",
                 strerror(errno));
        return -1;
    }
    /* Initialize boot_entry_t to zero. */
    memset(&entries[num-1], 0, sizeof(boot_entry_t));

    /* Fill the boot_prog_t. */
    snprintf(entries[num-1].prog.path, BOOTIMAGE_PATH, "%s", cmd);
    entries[num-1].prog.flags = flags;
    entries[num-1].prog.entry = entry;

    /* Done. */
    return 0;
}

int libboot_addseg(unsigned int base, unsigned int size, void *data)
{
    boot_entry_t *e = &entries[num-1];

    /* Enlarge boot_seg_t array for the current boot_entry_t. */
    if (!(e->segs = realloc(e->segs, (e->num_segs + 1) *
				      sizeof(boot_seg_t))))
    {
	snprintf(err_buf, sizeof(err_buf),
		"failed to realloc() boot_seg_t: %s",
		 strerror(errno));
	return -1;
    }
    /* Enlarge memory segment array. */
    if (!(e->data = realloc(e->data, (e->num_segs + 1) *
				      sizeof(void *))))
    {
	snprintf(err_buf, sizeof(err_buf),
		"failed to realloc() boot_entry_t.data: %s",
		 strerror(errno));
	return -1;
    }
    /* Allocate memory segment data buffer. */
    if (!(e->data[e->num_segs] = malloc(size)))
    {
	snprintf(err_buf, sizeof(err_buf),
		"failed to malloc() boot_entry_t.data: %s",
		 strerror(errno));
	return -1;
    }
    /* Copy memory segment data. */
    memcpy(e->data[e->num_segs], data, size);

    /* Initialize boot_seg_t. */
    e->segs[e->num_segs].virt_base = base;
    e->segs[e->num_segs].size      = size;
    e->num_segs++;
    total_segs++;

    /* Success. */
    return 0;
}

int libboot_write(char *path)
{
    FILE *fp;
    boot_image_t image;
    int i, j, nsegs = 0, data_off = 0;
    char zero[BOOTIMAGE_ALIGN];

    /* Initialize variables. */
    memset(zero, 0, sizeof(zero));

    /*
     * 1) align program segments at PAGE_SIZE boundaries.
     * 2) provide a stack segment containing program arguments
     *    in a null-separated list fashion.
     */
    if (!(fp = fopen(path, "w")))
    {
	snprintf(err_buf, sizeof(err_buf),
		"failed to fopen() `%s': %s",
		 path, strerror(errno));
	return -1;
    }
    /* Initialize boot image header. */
    image.magic[0] = BOOTIMAGE_MAGIC0;
    image.magic[1] = BOOTIMAGE_MAGIC1;
    image.revision = BOOTIMAGE_REVISION;
    image.var_table_off    = sizeof(boot_image_t);
    image.var_table_count  = 0;
    image.prog_table_off   = image.var_table_off +
			    (image.var_table_count * sizeof(boot_var_t));
    image.prog_table_count = num;
    image.seg_table_off    = image.prog_table_off +
			    (image.prog_table_count * sizeof(boot_prog_t));
    image.seg_table_count  = total_segs;

    /* Memory segment data starts after the segment tables. */
    data_off  = image.seg_table_off +
	       (image.seg_table_count * sizeof(boot_seg_t));
    data_off += BOOTIMAGE_ALIGN - (data_off % BOOTIMAGE_ALIGN);

    /* Write header to final image. */
    if (fseek(fp, 0, SEEK_SET) != 0 ||
        fwrite(&image, sizeof(image), 1, fp) != 1)
    {
	snprintf(err_buf, sizeof(err_buf),
		"failed to fwrite() boot_image_t: %s",
		 strerror(errno));
	fclose(fp);
	return -1;
    }
    /*
     * Loop boot_entry_t array.
     */
    for (i = 0; i < num; i++)
    {
	/* Update boot program entry. */
	entries[i].prog.segments_index = nsegs;
	entries[i].prog.segments_count = entries[i].num_segs;

        /* Write the boot program table entry. */
        if (fseek(fp, image.prog_table_off +
                 (i * sizeof(boot_prog_t)), SEEK_SET) != 0 ||
            fwrite(&entries[i].prog, sizeof(boot_prog_t), 1, fp) != 1)
        {
            snprintf(err_buf, sizeof(err_buf),
                    "failed to write to boot_prog_t: %s",
                     strerror(errno));
	    fclose(fp);
            return -1;
        }
	/* Loop program memory segments. */
	for (j = 0; j < entries[i].num_segs; j++)
	{
	    /* Update segment table entry. */
	    entries[i].segs[j].offset = data_off;

	    /* Write segment table entry. */
	    if (fseek(fp, image.seg_table_off +
	                 (nsegs * sizeof(boot_seg_t)), SEEK_SET) != 0 ||
		fwrite(&entries[i].segs[j], sizeof(boot_seg_t), 1, fp) != 1)
	    {
		snprintf(err_buf, sizeof(err_buf),
			"failed to write boot_seg_t: %s",
			 strerror(errno));
		fclose(fp);
		return -1;
	    }
	    /* Write segment data. */
	    if (fseek(fp, data_off, SEEK_SET) != 0 ||
		fwrite(entries[i].data[j], entries[i].segs[j].size, 1, fp) != 1)
	    {
	    	snprintf(err_buf, sizeof(err_buf),
			"failed to write boot_seg_t data: %s",
			 strerror(errno));
		fclose(fp);
		return -1;
	    }
	    data_off += entries[i].segs[j].size;

	    /* Append NULL-bytes to enforce alignment. */
	    if (fseek(fp, data_off, SEEK_SET) != 0 ||
	        fwrite(zero, BOOTIMAGE_ALIGN - (data_off % BOOTIMAGE_ALIGN),
	    		  1, fp) != 1)
	    {
		snprintf(err_buf, sizeof(err_buf),
			"failed to append boot_seg_t zeroes: %s",
			 strerror(errno));
		fclose(fp);
		return -1;
	    }
	    /* Increment counters. */
	    data_off += BOOTIMAGE_ALIGN - (data_off % BOOTIMAGE_ALIGN);
	    nsegs++;
	}
    }
    /* All done. */
    fclose(fp);
    libboot_reset();
    return 0;
}
