# *****************************************************************************
# * Makefile v.0.1 February, 13 2017                                          *
# * Copyright 2017 Giovanni Caini, Ossama Gana, Luca Morosini                 *
# *                                                                           *
# * This file is part of MiKABoO.                                             *
# *                                                                           *
# * MiKABoO is free software; you can redistribute it and/or modify it under  *
# * the terms of the GNU General Public License as published by the Free      *
# * Software Foundation; either version 2 of the License, or (at your option) *
# * any later version.                                                        *
# * This program is distributed in the hope that it will be useful, but       *
# * WITHOUT ANY WARRANTY; without even the implied warranty of                *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
# * Public License for more details.                                          *
# * You should have received a copy of the GNU General Public License along   *
# * with this program; if not, write to the Free Software Foundation, Inc.,   *
# * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.                  *
# *****************************************************************************

UARM_H		= include/listx.h /usr/include/uarm/libuarm.h /usr/include/uarm/uARMconst.h /usr/include/uarm/uARMtypes.h /usr/include/uarm/arch.h
PHASE1_H 	= include/mikabooq.h
CONST_H		= include/const.h
OBJECTS		= mikabooq.o p1test.o
UARM_LIBS	= /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o mika	 /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o

all: mika

p1: p1test.o mikabooq.o
	arm-none-eabi-ld -T $(UARM_LIBS)
	elf2uarm -k mika

mika: $(OBJECTS)
	arm-none-eabi-ld -T $(UARM_LIBS) $(OBJECTS)
	elf2uarm -k mika

mikabooq.o: mikabooq.c include/mikabooq.h
	arm-none-eabi-gcc -mcpu=arm7tdmi -c -Wall -I . -I "/usr/include/uarm" -I "/usr/include" -I "/usr/include/bits"-I "./include" -o mikabooq.o mikabooq.c

p1test.o: p1test.c $(UARM_H) $(PHASE1_H)
	arm-none-eabi-gcc -mcpu=arm7tdmi -c -Wall -I . -I "/usr/include/uarm" -I "/usr/include" -I "/usr/include/bits" -I "./include" -o p1test.o p1test.c

clean:
	rm -rf *o mika

cleanall:
	rm -rf *o mika mika.core.uarm mika.stab.uarm

