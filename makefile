CC=gcc
CFLAGS= -Wall -std=c11 -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes -Wunreachable-code

all: mpasswdsort

mpasswdsort: mpasswdsort.o list.o
	$(CC) -o mpasswdsort mpasswdsort.o list.o

mpasswdsort.o list.o: mpasswdsort.c list.c list.h
	$(CC) -c -g mpasswdsort.c list.c

clean:
	 rm -f rm mpasswdsort *.o