#include <err.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "display.h"

void grayscale(char *file)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // Initialize the SDL

    init_sdl();

    image_surface = load_image(file);
   
    // Display the image.
    
    screen_surface = display_image(image_surface);
	
    int width = image_surface->w;
    int height = image_surface->h;

    // Grayscale

    for(int x = 0; x < width; x++) {
	    for(int y = 0; y < height; y++) {
		    Uint32 pixel = get_pixel(image_surface, x, y);
		    Uint8 r, g, b;
		    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
		    r = average, g = average, b = average;
		    pixel = SDL_MapRGB(image_surface->format, r, g, b);
		    put_pixel(image_surface, x, y, pixel);
	    }
    }
    
    // Save the image
    SDL_SaveBMP(image_surface, "image_test/my_grey_image.bmp");
    
    // Update the surfaces
    update_surface(screen_surface, image_surface);

    // Wait for a key to be pressed.

    wait_for_keypressed();

    // Free the image surface.
    
    SDL_FreeSurface(image_surface);

    // Free the screen surface.

    SDL_FreeSurface(screen_surface);
}
