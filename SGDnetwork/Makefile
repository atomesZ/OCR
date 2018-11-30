CC = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O3

LIBS = -lm

all: main

main:  main.o network.o  stochastic.o
	$(CC) $(CFLAGS) -o main main.o network.o stochastic.o $(LIBS)

main.o:  main.c network.h backprop.h 
	$(CC) $(CFLAGS) -c main.c

network.o:  network.c network.h 
	$(CC) $(CFLAGS) -c network.c

# backprop.o:	backprop.c backprop.h
#	$(CC) $(CFLAGS) -c backprop.c

stochastic.o:	stochastic.c stochastic.h
	$(CC) $(CFLAGS) -c stochastic.c

clean: 
	$(RM) main *.o *~
