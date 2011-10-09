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

#ifndef __LIBBOOT_H
#define __LIBBOOT_H

/** First magic byte. */
#define BOOTIMAGE_MAGIC0	('F') + ('r' << 8) + \
			        ('e' << 16) + ('e' << 24)

/** Second magic byte. */
#define BOOTIMAGE_MAGIC1	('N') + ('O' << 8) + \
				('S' << 16) + (0x1 << 24)

/** Version of the boot image layout. */
#define BOOTIMAGE_REVISION	2

/** Maximum length of BootVariable keys. */
#define BOOTIMAGE_KEY		32

/** Maximum length of BootVariable values. */
#define BOOTIMAGE_VALUE		64

/** Maximum length of the command in a BootProgram. */
#define BOOTIMAGE_PATH		128

/** Alignment of memory segment data. */
#define BOOTIMAGE_ALIGN		4096

/**
 * @brief Boot Program Flags.
 * @{
 */

/**
 * Only schedule if no other programs are ready.
 * One copy is started on each CPU.
 */
#define BOOTPROG_ISIDLE (1 << 1)

/**
 * @}
 */

/**
 * BootImage contains executable programs to be loaded at system bootup.
 */
typedef struct boot_image
{
    unsigned int   magic[2];         /**<< Magic numbers to detect a valid boot image. */
    unsigned char  revision;         /**<< Version of the boot image layout. */
    unsigned int   checksum;	     /**<< Checksum used to verify integrity. */
    unsigned int   var_table_off;    /**<< Offset of the variables table in bytes. */
    unsigned short var_table_count;  /**<< Number of entries in the variables table. */
    unsigned int   prog_table_off;   /**<< Offset of the programs table in bytes. */
    unsigned short prog_table_count; /**<< Number of entries in the programs table. */
    unsigned int   seg_table_off;    /**<< Offset of the segments table in bytes. */
    unsigned short seg_table_count;  /**<< Number of entries in the segments table. */
}
boot_image_t;

/**
 * Simple key/value entry for inside the boot_image_t.
 * @see boot_image_t
 */
typedef struct boot_var
{
    unsigned char key[BOOTIMAGE_KEY];     /**<< Key is an uninterpreted ASCII string. */
    unsigned char value[BOOTIMAGE_VALUE]; /**<< Value is an uninterpreted ASCII string. */
}
boot_var_t;

/**
 * Program embedded in the BootImage.
 */
typedef struct boot_prog
{
    char path[BOOTIMAGE_PATH];     /**<< Path to the program. */
    unsigned short flags;	   /**<< Flags for this boot program. */
    unsigned int entry;    	   /**<< Program entry point. */
    unsigned int segments_index;   /**<< Index of the program segments
				         in the segments table. */
    unsigned short segments_count; /**<< Number of contiguous entries
				         in the segment table, including stack. */
    unsigned int stack_index;	   /**<< Index of the program stack segment
                                         also in the segments table. */
}
boot_prog_t;

/**
 * Program memory segment.
 */
typedef struct boot_seg
{
    unsigned int virt_base; /**<< Virtual memory address to load the segment. */
    unsigned int size;      /**<< Total size of the segment. */
    unsigned int offset;    /**<< Offset in the boot image of the segment contents. */
}
boot_seg_t;

/**
 * Initialize the boot image library.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_init(void);

/**
 * Deinitialize the boot image library.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_deinit(void);

/**
 * Retrieve a string presentation of the last occurred error.
 * @return String pointer.
 */
extern char * libboot_error(void);

/**
 * Parse an in-memory boot image.
 * @param image Pointer to an in-memory boot image.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_parse(boot_image_t *image);

/**
 * Read the next boot program from the in-memory boot image.
 * @return Pointer to a boot_prog_t structure.
 */
extern boot_prog_t * libboot_getprog(void);

/**
 * Indicates to repeat reading the current program.
 */
extern void libboot_repeat_prog(void);

/**
 * Read the next memory segment for the current boot program.
 * @return Pointer to a boot_seg_t.
 */
extern boot_seg_t * libboot_getseg(void);

/**
 * Read the memory segment containing the initial program stack.
 * @return Pointer to a boot_seg_t.
 */
extern boot_seg_t * libboot_getstack(void);

/**
 * Insert a new program in the boot image for writing.
 * @param cmd Full command of the program including arguments.
 * @param entry Entry point of the program.
 * @param flags Optional boot program flags.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_addprog(char *cmd, unsigned int entry,
				      unsigned short flags);

/**
 * Add a memory region to a boot program.
 * @param base Virtual memory address of the region.
 * @param size Size of the memory region in bytes.
 * @param void Pointer to the data of the memory region.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_addseg(unsigned int base, unsigned int size, void *data);

/**
 * Output the final boot image to a file.
 * @param path Path to the output file.
 * @return Zero on success and non-zero on failure.
 */
extern int libboot_write(char *path);

#endif /* __LIBBOOT_H */

