
CC=gcc
CFLAGS=-g -Wall -Wextra -std=gnu99
#CFLAGS=-std=gnu99 -O3

#CC=clang
#CFLAGS=-g -Wall -Wextra -std=gnu99 -fno-omit-frame-pointer -fsanitize=address

.PHONY: clean all

library: 2048.c 2048.h
	$(CC) $(CFLAGS) -c 2048.c
	ar rcs lib2048.a 2048.o


nc: nc2048.c nc2048.h library
	$(CC) $(CFLAGS) nc2048.c -lcurses -l2048 -L. -o nc2048


tests: tst2k.c library
	$(CC) $(CFLAGS) tst2k.c -lm -L. -l2048 -o tst2k

all: nc

clean:
	rm -f *.o lib2048.a
	rm nc2048
