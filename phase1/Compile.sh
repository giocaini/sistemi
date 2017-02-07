arm-none-eabi-gcc -c -I /usr/include/uarm/ -mcpu=arm7tdmi p1test.c -o p1test.o
arm-none-eabi-gcc -c -I /usr/include/uarm/ -mcpu=arm7tdmi mikabooq.c -o mikabooq.o
arm-none-eabi-ld -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o p1test /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o p1test.o mikabooq.o
elf2uarm -k p1test
uarm
