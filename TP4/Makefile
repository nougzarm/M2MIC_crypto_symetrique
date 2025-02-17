PROG = TP4

OBJS = TP3.o

CC = gcc
CPFLAGS = -Wall -Wpointer-arith -O2


TP3: TP4.o
	$(CC) $(CPFLAGS) -o TP4  TP4.o

test.o: TP4.c TP4.h
	$(CC) $(CPFLAGS) -c TP4.c

clean:
	rm -f $(OBJS) $(PROG)