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

#include <FreeNOS/process.h>

.global context_switch
.section ".text"

/*
 * context_switch(old_stack, old_flags, page_dir, kern_stack)
 */
context_switch:

    /* Setup correct stackframe. */
    push %ebp
    mov  %esp, %ebp

    /* Saves the current process state, if any. */
    mov 8(%ebp), %eax
    cmpl $0, %eax
    je restore_state

    /* Setup stack for IRETD later on. */
    pushf
    push %cs
    push $resume

    /* Save registers on the current stack. */
    pushl $0
    pushl $0
    pusha
    pushl %ss
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    /* Save stack pointer of current process in old_stack. */
    pushl %esp
    popl %ebx
    movl %ebx, (%eax)

    /* Unmark the current process running, for rescheduling later. */
    movl 12(%ebp), %eax
    movl (%eax), %ebx
    andl $(~PROC_RUNNING), %ebx
    movl %ebx, (%eax)

    /*
     * Switch context to new process.
     */
restore_state:

    /* Retrieve arguments. */
    movl 16(%ebp), %eax /* page_dir */
    movl 20(%ebp), %ebx /* stack */

    /* Reload page directory and stack. */
    movl %eax,     %cr3
    movl %ebx,     %esp

    /* Restore CPU registers. */
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popl %ss
    popa
    addl $8, %esp

    /* Resume execution. */
    iret

resume:
    pop %ebp
    ret

