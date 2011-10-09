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

#ifndef __LIBC_STDIO_H
#define __LIBC_STDIO_H

#include <sys/types.h>
#include "stddef.h"
#include "stdarg.h"

/**
 * @defgroup libc libc (ISO C99)
 * @{
 */

/**
 * @brief Seek operations
 * @{
 */

/** Seek relative to current position. */
#define SEEK_CUR	0

/** Seek relative to end-of-file. */
#define SEEK_END	1

/** Seek relative to start-of-file. */
#define SEEK_SET	2

/**
 * @}
 */

/**
 * @brief File streams
 * @{
 */

/**
 * A structure containing information about a file.
 */
typedef struct FILE
{
    /** File descriptor. */
    int fd;
}
FILE;

/**
 * @brief Open a stream.
 *
 * The fopen() function shall open the file whose pathname is the
 * string pointed to by filename, and associates a stream with it.
 * The mode argument points to a string. If the string is one of the
 * following, the file shall be opened in the indicated mode. Otherwise,
 * the behavior is undefined.
 *
 * r or rb           Open file for reading.
 * w or wb           Truncate to zero length or create file for writing.
 * a or ab           Append; open or create file for writing at end-of-file.
 * r+ or rb+ or r+b  Open file for update (reading and writing).
 * w+ or wb+ or w+b  Truncate to zero length or create file for update.
 * a+ or ab+ or a+b  Append; open or create file for update, writing at end-of-file.
 *
 * @param filename Path to the file to open.
 * @param mode Mode describes how to open the file.
 * @return Upon successful completion, fopen() shall return a pointer
 *         to the object controlling the stream. Otherwise, a null
 *         pointer shall be returned, and errno shall be set to indicate the error.
 */
extern FILE * fopen(const char *filename,
		    const char *mode);

/**
 * @brief Binary input.
 *
 * The fread() function shall read into the array pointed to by ptr
 * up to nitems elements whose size is specified by size in bytes,
 * from the stream pointed to by stream. For each object, size calls
 * shall be made to the fgetc() function and the results stored, in
 * the order read, in an array of unsigned char exactly overlaying
 * the object. The file position indicator for the stream (if defined)
 * shall be advanced by the number of bytes successfully read. If an
 * error occurs, the resulting value of the file position indicator
 * for the stream is unspecified. If a partial element is read, its
 * value is unspecified.
 *
 * @param ptr Output buffer.
 * @param size Size of each item to read.
 * @param nitems Number of items to read.
 * @param stream FILE pointer to read from.
 * @return Upon successful completion, fread() shall return the number
 *         of elements successfully read which is less than nitems only
 *         if a read error or end-of-file is encountered. If size or
 *         nitems is 0, fread() shall return 0 and the contents of the
 *         array and the state of the stream remain unchanged. Otherwise,
 *         if a read error occurs, the error indicator for the stream shall
 *         be set, and errno shall be set to indicate
 *         the error.
 */
extern size_t fread(void *ptr, size_t size,
		    size_t nitems, FILE *stream);

/**
 * @brief Binary output.
 *
 * The fwrite() function shall write, from the array pointed to by ptr,
 * up to nitems elements whose size is specified by size, to the stream
 * pointed to by stream. For each object, size calls shall be made to
 * the fputc() function, taking the values (in order) from an array of
 * unsigned char exactly overlaying the object. The file-position indicator
 * for the stream (if defined) shall be advanced by the number of bytes
 * successfully written. If an error occurs, the resulting value of the
 * file-position indicator for the stream is unspecified.
 *
 * @param ptr Input buffer.
 * @param size Size of each item to write.
 * @param nitems Number of items to write.
 * @param stream FILE pointer to write to.
 * @return The fwrite() function shall return the number of elements
 *         successfully written, which may be less than nitems if a
 *         write error is encountered. If size or nitems is 0, fwrite()
 *         shall return 0 and the state of the stream remains unchanged.
 *         Otherwise, if a write error occurs, the error indicator for
 *         the stream shall be set,
 */
extern size_t fwrite(void *ptr, size_t size,
		     size_t nitems, FILE *stream);

/**
 * @brief Reposition a file-position indicator in a stream.
 *
 * The fseek() function shall set the file-position indicator
 * for the stream pointed to by stream. If a read or write error
 * occurs, the error indicator for the stream shall be set and
 * fseek() fails.
 *
 * The new position, measured in bytes from the beginning of the
 * file, shall be obtained by adding offset to the position
 * specified by whence. The specified point is the beginning
 * of the file for SEEK_SET, the current value of the file-position
 * indicator for SEEK_CUR, or end-of-file for SEEK_END.
 *
 * If the stream is to be used with wide-character input/output
 * functions, the application shall ensure that offset is either
 * 0 or a value returned by an earlier call to ftell() on the same
 * stream and whence is SEEK_SET.
 *
 * A successful call to fseek() shall clear the end-of-file indicator
 * for the stream and undo any effects of ungetc() and ungetwc() on
 * the same stream. After an fseek() call, the next operation on an
 * update stream may be either input or output.
 *
 * If the most recent operation, other than ftell(), on a given stream
 * is fflush(), the file offset in the underlying open file description
 * shall be adjusted to reflect the location specified by fseek().
 *
 * The fseek() function shall allow the file-position indicator to be
 * set beyond the end of existing data in the file. If data is later
 * written at this point, subsequent reads of data in the gap shall
 * return bytes with the value 0 until data is actually written into
 * the gap.
 *
 * The behavior of fseek() on devices which are incapable of seeking is
 * implementation-defined. The value of the file offset associated with
 * such a device is undefined.
 *
 * If the stream is writable and buffered data had not been written to
 * the underlying file, fseek() shall cause the unwritten data to be
 * written to the file and shall mark the last data modification and
 * last file status change timestamps of the file for update.
 *
 * In a locale with state-dependent encoding, whether fseek() restores
 * the stream's shift state is implementation-defined.
 *
 * @param stream File stream pointer to seek in.
 * @param offset New offset value.
 * @param whence Either SEEK_SET, SEEK_CUR or SEEK_END.
 * @return The fseek() and fseeko() functions shall return 0 if they succeed.
 *         Otherwise, they shall return -1 and set errno to indicate the error.
 */
extern int fseek(FILE *stream, long offset, int whence);

/**
 * @brief Close a stream.
 *
 * The fclose() function shall cause the stream pointed to by stream
 * to be flushed and the associated file to be closed. Any unwritten
 * buffered data for the stream shall be written to the file; any unread
 * buffered data shall be discarded. Whether or not the call succeeds, the
 * stream shall be disassociated from the file and any buffer set by the setbuf()
 * or setvbuf() function shall be disassociated from the stream. If the associated
 * buffer was automatically allocated, it shall be deallocated.
 *
 * @param stream File stream to close.
 * @return Upon successful completion, fclose() shall return 0; otherwise,
 *         it shall return EOF and set errno to indicate the error.
 */
extern int fclose(FILE *stream);

/**
 * @}
 */

/**
 * Output a debug message using printf().
 * @param fmt Formatted string.
 * @param ... Argument list.
 * @see printf
 */
#define dprintf(fmt, ...) \
    printf("{%s:%d}: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

/**
 * Write a formatted string into a buffer.
 * @param buffer String buffer to write to.
 * @param size Maximum number of bytes to write.
 * @param fmt Formatted string.
 * @param ... Argument list.
 * @return Number of bytes written to the buffer.
 */
extern int snprintf(char *buffer, unsigned int size, const char *fmt, ...);

/**
 * Write a formatted string into a buffer.
 * @param buffer String buffer to write to.
 * @param size Maximum number of bytes to write.
 * @param fmt Formatted string.
 * @param args Argument list.
 * @return Number of bytes written to the buffer.
 */
extern int vsnprintf(char *buffer, unsigned int size,
		     const char *fmt, va_list args);

/**
 * Output a formatted string to standard output.
 * @param format Formatted string.
 * @param ... Argument list.
 * @return Number of bytes written or error code on failure.
 */
extern int printf(const char *format, ...);

/**
 * Output a formatted string to standard output, using a variable argument list.
 * @param format Formatted string.
 * @param ... Argument list.
 * @return Number of bytes written or error code on failure.
 */
extern int vprintf(const char *format, va_list args);

/**
 * @}
 */

#endif /* __LIBC_STDIO_H */

