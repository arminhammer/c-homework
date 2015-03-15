# macros

CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=grand attacker

all: grand attacker

grand:
	$(CC) $(CFLAGS) -o grand grand.c

attacker: attacker.c
	$(CC) $(CFLAGS) -o attacker attacker.c

clean:
	rm -f $(EXECUTABLES) *.o *.a *~ *.s a.out *.so