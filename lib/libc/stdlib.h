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

#ifndef __LIBC_STDLIB_H
#define __LIBC_STDLIB_H

#include <sys/types.h>

/**
 * @defgroup libc libc (ISO C99)
 * @{
 */

/** Successful termination. */
#define EXIT_SUCCESS 0

/** Unsuccessful termination. */
#define EXIT_FAILURE 1

/**
 * @brief Terminate a process.
 *
 * The exit function causes normal program termination to occur.
 * If more than one call to the exit function is executed by a program,
 * the behaviour is undefined.
 *
 * @param status If the value of status is zero or EXIT_SUCCESS, an
 *		 implementation defined form of the status successful
 *		 termination is returned. If the value of status is
 *		 EXIT_FAILURE, an implementation-defined form of the status
 *		 unsuccesful termination is returned. Otherwise the status
 *		 is implementation-defined.
 * @return The exit function cannot return to its caller.
 */
extern void exit(int status);

/**
 * @brief Convert a number to a string.
 * @param buffer String buffer to write to.
 * @param divisor Base of the number, e.g. 16 for hexadecimal.
 * @param number The number to convert.
 */
extern void itoa(char *buffer, int divisor, int number);

/**
 * @brief Convert a string to an integer.
 *
 * The atoi, atol, and atoll functions convert the initial portion of the string
 * pointed to by nptr to int, long int, and long long int representation,
 * respectively.
 *
 * @param nptr String pointer to convert.
 * @return The atoi, atol, and atoll functions return the converted value.
 */
extern int atoi(const char *nptr);

/**
 * @brief Convert a string to a long integer
 *
 * These functions shall convert the initial portion of the string pointed to
 * by str to a type long and long long representation, respectively. First,
 * they decompose the input string into three parts:
 *
 * @param nptr Input string pointer to convert.
 * @param endptr If the subject sequence is empty or does not have the
 *               expected form, no conversion is performed; the value of
 *               str is stored in the object pointed to by endptr, provided
 *               that endptr is not a null pointer.
 * @param base Base of the number, e.g. 16 for hexadecimal.
 * @return Upon successful completion, these functions shall return the
 *         converted value, if any. If no conversion could be performed,
 *         0 shall be returned and errno may be set to [EINVAL]. If the
 *         correct value is outside the range of representable values,
 *         {LONG_MIN}, {LONG_MAX}, {LLONG_MIN}, or {LLONG_MAX} shall be
 *         returned (according to the sign of the value), and errno set
 *         to [ERANGE].
 */
extern long strtol(const char *nptr, char **endptr, int base);

/**
 * @brief A memory allocator
 *
 * The malloc() function shall allocate unused space for an object whose
 * size in bytes is specified by size and whose value is unspecified.
 *
 * @param size Number of bytes to allocate.
 * @return Upon successful completion with size not equal to 0, malloc()
 *         shall return a pointer to the allocated space. If size is 0,
 *         either a null pointer or a unique pointer that can be
 *         successfully passed to free() shall be returned. Otherwise,
 *         it shall return a null pointer [CX] [Option Start]  and set
 *         errno to indicate the error.
 */
extern void * malloc(size_t size);

/**
 * @brief Memory reallocator
 *
 * The realloc() function shall change the size of the memory object
 * pointed to by ptr to the size specified by size. The contents of
 * the object shall remain unchanged up to the lesser of the new and
 * old sizes. If the new size of the memory object would require
 * movement of the object, the space for the previous instantiation
 * of the object is freed. If the new size is larger, the contents
 * of the newly allocated portion of the object are unspecified.
 * If size is 0 and ptr is not a null pointer, the object pointed
 * to is freed. If the space cannot be allocated, the object shall
 * remain unchanged.
 *
 * If ptr is a null pointer, realloc() shall be equivalent to
 * malloc() for the specified size. If ptr does not match a
 * pointer returned earlier by calloc(), malloc(), or realloc()
 * or if the space has previously been deallocated by a call to
 * free() or realloc(), the behavior is undefined.
 *
 * The order and contiguity of storage allocated by successive
 * calls to realloc() is unspecified. The pointer returned if
 * the allocation succeeds shall be suitably aligned so that
 * it may be assigned to a pointer to any type of object and
 * then used to access such an object in the space allocated
 * (until the space is explicitly freed or reallocated). Each
 * such allocation shall yield a pointer to an object disjoint
 * from any other object. The pointer returned shall point to
 * the start (lowest byte address) of the allocated space. If
 * the space cannot be allocated, a null pointer shall be returned.
 *
 * @param ptr Pointer to previously allocated memory by malloc()
 *            or realloc(), or NULL if no memory allocated yet.
 * @param size Size of the memory to (re)allocate.
 * @return Upon successful completion with a size not equal to 0,
 *         realloc() shall return a pointer to the (possibly moved)
 *         allocated space. If size is 0, either a null pointer or
 *         a unique pointer that can be successfully passed to free()
 *         shall be returned. If there is not enough available memory,
 *         realloc() shall return a null pointer and set errno to ENOMEM.
 */
extern void * realloc(void *ptr, size_t size);

/**
 * @brief Free allocated memory
 *
 * The free() function shall cause the space pointed to by ptr to be
 * deallocated; that is, made available for further allocation. If ptr
 * is a null pointer, no action shall occur. Otherwise, if the argument
 * does not match a pointer earlier returned by a function in POSIX.1-2008
 * that allocates memory as if by malloc(), or if the space has been
 * deallocated by a call to free() or realloc(), the behavior is undefined.
 * Any use of a pointer that refers to freed space results in undefined behavior.
 *
 * @param ptr Previously allocated memory to free.
 * @return The free() function shall not return a value.
 */
extern void free(void *ptr);

/**
 * @}
 */

#endif /* __LIBC_STDLIB_H */

