/* Copyright (C) 1993 Sakamura Lab, the University of Tokyo, JAPAN
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

	.globl	start
	.text
start:
	mov r0, r11		/* save r0-r1 to r11-r12 (for ItIs task) */
	mov r1, r12
	mov #__bss_start, r1
	mov #__end, r2
	subu r1, r2
	ble __startup_1
	mov.b #0, r3
	sstr.b
__startup_1:
	/*
	 *  To produce ROMable executable program, contents of the
	 *  DATA section are added to the TEXT section.  __idata_start
	 *  holds the first address of the copied DATA section.  Here,
	 *  we copy the copied DATA section into their right place.
	 */
	mov #__idata_end, r2
	sub #__idata_start, r2
	ble __startup_2
	mov #__idata_start, r0
	mov #__data_start, r1
	smov/f/n.b
__startup_2:
	mov #0, r7		/* default 1st argument for main() */
	mov #0, r8		/*	   2nd			   */
	mov #0, r9		/*	   3rd			   */
	mova @__startup_3, r0	/* enter return address in r0 */
	bra ___sys_init		/* __sys_init(); */
__startup_3:
	mov r9, @___environ
	mov r7, r0		/* __libc_init(r7, r8, r9); */
	mov r8, r1
	mov r9, r2
	bsr ___libc_init
	mov r7, r0		/* ercd = main(r7, r8, r9); */
	mov r8, r1
	mov r9, r2
	bsr _main
	bsr _exit		/* exit(ercd); */

	.comm _errno,4
	.comm ___environ,4
