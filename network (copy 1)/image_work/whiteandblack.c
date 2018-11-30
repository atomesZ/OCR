#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

Uint32 otsu(SDL_Surface* image_surface) {

	int threshold = 0;

	// Calculate histogram
	
	int width = image_surface->w;
	int height = image_surface->h;

	Uint8 array[256];

	for(int x = 0; x <= width; x++) {
		for(int y = 0; y <= height; y++) { 
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
		       	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			array[b] += 1;
		}
	}
	
	double nb_total = width * height;

	// Sum of pixels weight

	double sum = 0;

	for(int i = 0; i < 256; i++) {
		sum += i * array[i];
	}

	float varMax = 0;
	float sumB = 0;
	float sumF = 0;
	double sB = 0;
	double sF = 0;
	double wB = 0;
	double wF = 0;
	
	for(Uint32 i = 0; i < 256; i++) {
		
		// Weight Background
		sB += array[i];

		// Weight Foreground
		sF += array[255 - i];

		wB = sB / nb_total;
		wF = sF / nb_total;

		sumB += i * array[i];
		sumF += (255 - i) * array[255 - i];

		// Mean Background
		float mB = sumB / sB;

		// Mean Foreground
		float mF = sumF / sF;

		// Class Between Class Variance
		float bVar = (float)wB * (float)wF * (mB - mF) * (mB - mF);

		// Check new maximum
		if (bVar > varMax) {
			varMax = bVar;
			threshold = i;
		}

	}

	return threshold;
}

void whiteandblack(char *path)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // Initialize the SDL
    init_sdl();

    // Load the image
    image_surface = load_image(path);
   
    // Display the image
    screen_surface = display_image(image_surface);
	
    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 t = otsu(image_surface);

    // Black or white

    for(int x = 0; x < width; x++) {

	    for(int y = 0; y < height; y++) {

		    // Get the pixel value
		    Uint32 pixel = get_pixel(image_surface, x, y);
		    
		    // Get the RGB values
		    Uint8 r, g, b;
		    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		    
		    // Update the pixel based on Otsu threshold
		    Uint8 black_white = r < t ? 0 : 255;
		    pixel = SDL_MapRGB(image_surface->format, black_white, black_white, black_white);
		    put_pixel(image_surface, x, y, pixel);
	    }
    }

    // Save the imgae
    SDL_SaveBMP(image_surface, "image_test/my_blackandwhite_image.bmp");

    // Update the surfaces
    update_surface(screen_surface, image_surface);

    // Wait for a key to be pressed.
    wait_for_keypressed();

    // Free the image surface.
    SDL_FreeSurface(image_surface);

    // Free the screen surface.
    SDL_FreeSurface(screen_surface);
}

