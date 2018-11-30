# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all:display grayscale whiteandblack rlsa main interface

display:display.o
grayscale: pixel_operations.o grayscale.o
whiteandblack: pixel_operations.o whiteandblack.o
rlsa: pixel_operations.o rlsa.o
main: interface.o display.o pixel_operations.o

grayscale.o: pixel_operations.h
whiteandblack.o: pixel_operations.h
rlsa.o: pixel_operations.h
interface: display.h pixel_operations.h
main: display.h pixel_operations.h interface.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
	${RM} whiteandblack
	${RM} rlsa ${RM} interface ${RM} main

# END
