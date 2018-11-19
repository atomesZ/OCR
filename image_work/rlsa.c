#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}


SDL_Surface* rlsa(SDL_Surface *img) {

	int hor_thres = 1000;
       	int zero_count = 0;
       	int one_flag = 0;
       	
	for (int i = 0; i<(img->w); i++) {
	 
		for (int j = 0; j<(img->h); j++) {

		       	Uint32 pixel = get_pixel(img, i, j);
		       	Uint8 r, g, b;
		       	SDL_GetRGB(pixel, img->format, &r, &g, &b);
		       	
			if (r == 255) {

			       	if (one_flag == 255) {

				       	if (zero_count <= hor_thres) {

					       	for (int a = j-zero_count; a<=j; a++) {

						       	Uint32 pixel = get_pixel(img, i, a);
						       	Uint8 r, g, b;
							r = 255, g = 255, b = 255;
							pixel = SDL_MapRGB(img->format, r, g, b);
							put_pixel(img, i, a, pixel);
					       	}
				       	} 
					
					else {
					       	one_flag = 0;
				       	}
				       	zero_count = 0;
			       	}
			       	one_flag = 255;
		       	}
		       	else { 
				if (one_flag == 0) {
				       	zero_count = zero_count + 1;
			       	}
		       	}
	       	}
       	}

	return img;
}



int main() {
	
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

	// Initialize the SDL
	init_sdl();

	// Load the image
	image_surface = load_image("image_test/my_blackandwhite_image.bmp");
       
	// Display the image
	screen_surface = display_image(image_surface);

	// Update the surfaces	
	update_surface(screen_surface, image_surface);
	
       	// Wait for a key to be pressed.
	wait_for_keypressed();

    	// Free the image surface.   
	SDL_FreeSurface(image_surface);
   
       	// Free the screen surface.
       	SDL_FreeSurface(screen_surface);


	return 0;

}
