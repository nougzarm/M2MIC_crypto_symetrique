PROG = TP3

OBJS = TP3.o

CC = gcc
CPFLAGS = -Wall -Wpointer-arith -O2


TP3: TP3.o
	$(CC) $(CPFLAGS) -o TP3  TP3.o

test.o: TP3.c TP3.h
	$(CC) $(CPFLAGS) -c TP3.c

clean:
	rm -f $(OBJS) $(PROG)