# macros

CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=ihttpd attacker1

all: clean ihttpd rev

ihttpd:
	$(CC) $(CFLAGS) -o ihttpd -pthread ihttpd.c stringutil.c http.c config.c -ldl

clean:
	rm -f $(EXECUTABLES) *.o *.a *~ *.s a.out *.so
	rm -f mod/rev.so

rev:
	$(CC) -c -fPIC rev.c
	$(CC) -shared rev.o -o rev.so
	mv rev.so mod/
	
attacker1: attacker1.c
	$(CC) $(CFLAGS) -o attacker1 attacker1.c
