CC = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c99 -O3 -lm

default: main

main:  main.o network.o 
	$(CC) $(CFLAGS) -o main main.o network.o

main.o:  main.c network.h  
	$(CC) $(CFLAGS) -c main.c

network.o:  network.c network.h 
	$(CC) $(CFLAGS) -c network.c

clean: 
	$(RM) main *.o *~