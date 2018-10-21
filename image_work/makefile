# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all:display grayscale whiteandblack

display:display.o
grayscale: pixel_operations.o grayscale.o
whiteandblack: pixel_operations.o whiteandblack.o

grayscale.o:pixel_operations.h
whiteandblack.o:pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
	${RM} whiteandblack

# END
