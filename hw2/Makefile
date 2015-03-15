# macros

CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=speller attacker1 attacker2 hashtest listtest

all: clean speller attacker1 attacker2 

speller:
	$(CC) $(CFLAGS) -o speller speller.c stringutil.c list.c hash.c

attacker1: attacker1.c
	$(CC) $(CFLAGS) -o attacker1 attacker1.c

attacker2: attacker2.c
	$(CC) $(CFLAGS) -o attacker2 attacker2.c

clean:
	rm -f $(EXECUTABLES) *.o *.a *~ *.s a.out *.so

hash:
	$(CC) $(CFLAGS) hash.c list.c -o hashtest

list:
	$(CC) $(CFLAGS) list.c -o listtest
