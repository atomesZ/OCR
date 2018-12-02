# Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all:main

display:display.o
grayscale: pixel_operations.o grayscale.o
whiteandblack: pixel_operations.o whiteandblack.o
rlsa: pixel_operations.o rlsa.o
main: interface.o display.o rlsa.o pixel_operations.o
backprop: backprop.o
network.o: network.o

grayscale.o: pixel_operations.h
whiteandblack.o: pixel_operations.h
backprop.o: backprop.h
network.o: network.h 
rlsa: pixel_operations.h
interface: display.h pixel_operations.h
main: display.h pixel_operations.h interface.h rlsa.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END