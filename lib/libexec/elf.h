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

#ifndef __LIBEXEC_ELF_H
#define __LIBEXEC_ELF_H

#include "libexec.h"
#include "elf_header.h"

/**
 * @defgroup libexec_elf libexec (ELF)
 * @{
 */

/**
 * Executable and Linkable Format (ELF) handler.
 * @param fp File handle to the program to parse.
 * @param program On output write the parsed program pointer here.
 * @return LIBEXEC_FORMAT_OK on success and LIBEXEC_FORMAT_ERROR on error.
 */
extern int elf_read(FILE *fp, program_t **program);

/**
 * @}
 */

#endif /* __LIBEXEC_ELF_H */

