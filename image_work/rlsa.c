#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

SDL_Surface* rlsa(SDL_Surface *img) {

	int width = img->w;
	int height = img->h;
	int threshold = 10;
	int zero_c = 0;

	for(int y = 0; y < height; y++) {

		Uint32 pixel = get_pixel(img, x, y);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

		int x = 0;

		while(r == 0)
			x++;
		x++;
		
		while(x < width) {
			while(r == 0) {
				xero_c++;
				x++;
			}

			if (xero_c < threshold) {
				x -= xero_c;

				while (xero_c != 0) {
					r, g, b = 255;
					pixel = SDL_MapRGB(img->
					xero_c--;
				}
			}

			else {
				xero_c = 0;
				x++;
			}
		}

		
			
	}
	

	









int main() {


	return 0;

}
