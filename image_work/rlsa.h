#ifndef RLSA_H_
#define RLSA_H_

#include <stdlib.h>
#include "SDL/SDL.h"

void SDL_to_matrix(SDL_Surface *img, double *mat);
void rlsa_SDLup(SDL_Surface *img, int thrs);
void rlsa_SDLdown(SDL_Surface *img, int thrs);
void rlsa_SDLleft(SDL_Surface *img, int thrs);
void rlsa_SDLright(SDL_Surface *img, int thrs);
void rlsalc(SDL_Surface *img1, SDL_Surface *img2, SDL_Surface *imgr);
void print_mat(double *mat, int rows, int cols);
void rlsa(char *file);
void create_dataset(char* filename, double *m, char value);

#endif
