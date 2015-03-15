# macros

CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=arminsh stacktest attacker1 attacker2

all: clean arminsh

arminsh:
	$(CC) $(CFLAGS) -o arminsh arminsh.c stringutil.c stack.c dirutil.c forkutil.c
	
stacktest:
	$(CC) $(CFLAGS) -o stacktest stack.c

clean:
	rm -f $(EXECUTABLES) *.o *.a *~ *.s a.out *.so

attacker1: attacker1.c
	$(CC) $(CFLAGS) -o attacker1 attacker1.c

attacker2: attacker2.c
	$(CC) $(CFLAGS) -o attacker2 attacker2.c
