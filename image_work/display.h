#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdlib.h>
#include "SDL/SDL.h"

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
int wait_for_keypressed();
void display(char *file);
void grayscale(char *file);
void whiteandblack(char *file);

#endif
