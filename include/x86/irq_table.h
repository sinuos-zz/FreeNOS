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

#ifndef __IRQ_TABLE_H
#define __IRQ_TABLE_H

#ifdef __ASSEMBLER__

#define IRQ(vector) \
	.global i##vector; \
	i##vector: \
		pushl $0; \
		pushl $vector; \
		jmp irq_execute;

#define IRQ_E(vector) \
	.global i##vector; \
	i##vector: \
		pushl $vector; \
		jmp irq_execute;

#else /* __ASSEMBLER__ */

extern void i0(void);
extern void i1(void);
extern void i2(void);
extern void i3(void);
extern void i4(void);
extern void i5(void);
extern void i6(void);
extern void i7(void);
extern void i8(void);
extern void i9(void);
extern void i10(void);
extern void i11(void);
extern void i12(void);
extern void i13(void);
extern void i14(void);
extern void i15(void);
extern void i16(void);
extern void i32(void);
extern void i33(void);
extern void i34(void);
extern void i35(void);
extern void i36(void);
extern void i37(void);
extern void i38(void);
extern void i39(void);
extern void i40(void);
extern void i41(void);
extern void i42(void);
extern void i43(void);
extern void i44(void);
extern void i45(void);
extern void i46(void);
extern void i47(void);
extern void i48(void);
extern void i144(void);

#endif /* __ASSEMBLER__ */
#endif /* __IRQ_TABLE_H */

