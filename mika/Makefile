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
ARM_INC		= arm-none-eabi-gcc -mcpu=arm7tdmi -c -Wall -I . -I "/usr/include/i386-linux-gnu" -I "/usr/include/uarm" -I "/usr/include" -I "./include" -include stdint.h -o
PHASE1_H 	= include/mikabooq.h
CONST_H		= include/const.h
OBJECTS		= ./bin/mikabooq.o ./bin/p1test.o
UARM_LIBS	= /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o mika	 /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o

all: makedir mika

makedir:
	mkdir bin
	mkdir kernel

mika: mikabooq.o p1test.o
	arm-none-eabi-ld -T $(UARM_LIBS) $(OBJECTS)
	elf2uarm -k mika
	mv mika.core.uarm ./kernel/mika.core.uarm
	mv mika.stab.uarm ./kernel/mika.stab.uarm
	rm mika
	rm -r bin

mikabooq.o: src/mikabooq.c include/mikabooq.h
	$(ARM_INC) ./bin/mikabooq.o src/mikabooq.c

p1test.o: src/p1test.c $(UARM_H) $(PHASE1_H)
	$(ARM_INC) ./bin/p1test.o src/p1test.c

clean:
	rm -r kernel

