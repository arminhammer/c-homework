# macros

CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=fib ifib rfib attacker

all: fib attacker

fib:
	$(CC) $(CFLAGS) -o fib fib.c
	ln -s fib rfib
	ln -s fib ifib

attacker: attacker.c
	$(CC) $(CFLAGS) -o attacker attacker.c

clean:
	rm -f $(EXECUTABLES) *.o *.a *~ *.s a.out *.so
