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

#ifndef __SMP_H
#define __SMP_H

#include "multiboot.h"
#include "types.h"

/** Maximum number of CPU's supported. */
#define SMP_MAX 256

/** Multiprocessor Floating Structure Signature. */
#define MP_FLOAT_SIGNATURE  0x5f504d5f

/** Multiprocessor Configuration Type ID for processors. */
#define MP_ENTRY_PROC 0

/** Physical memory address for smp_entry(). */
#define MP_BOOTENTRY_ADDR 0xf000

#include <kernel/smp.h>

/**
 * Multiprocessor Floating Structure.
 */
typedef struct mp_float
{
    unsigned long signature;
    unsigned long config_addr;
    unsigned char length;
    unsigned char revision;
    unsigned char checksum;
    unsigned char feature1;
    unsigned long feature2;
}
mp_float_t;

/**
 * Multiprocessor Configuration Structure.
 */
typedef struct mp_config
{
    char signature[4];
    unsigned short length;
    unsigned char revision;
    unsigned char checksum;
    char oem_id[8];
    char product_id[12];
    unsigned long oem_addr;
    unsigned short oem_length;
    unsigned short count;
    unsigned long apic_addr;
    unsigned long reserved;
}
mp_config_t;

/**
 * Multiprocessor Configuration Entry.
 */
typedef struct mp_entry
{
    unsigned char type;
    unsigned char apic_id;
    unsigned char apic_revision;
    unsigned char cpu_flags;
    u32 signature;
    u32 features;
    u32 reserved[2];
}
mp_entry_t;

/**
 * Scan memory pages for a valid Multiprocessor Configuration structure.
 * @param addr Start address to search in.
 * @param count Number of bytes to traverse during the search.
 * @return Pointer to a Multiprocessor Configuration or NULL on failure.
 */
extern mp_config_t * smp_scan(addr_t addr, size_t count);

/**
 * Startup a CPU.
 * @param cpu Processor to start.
 */
extern void smp_boot(cpu_t *cpu);

/**
 * Entry point when starting a new CPU.
 */
extern void smp_entry(void);

/** Stack (segment) pointer used in entry32.S . */
extern addr_t stackptr;

#endif /* __SMP_H */

