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

int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // Initialize the SDL
    init_sdl();

    // Load the image
    image_surface = load_image("image_test/my_grey_image.bmp");
   
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
		    
		    //Uint8 black_white = (r + g + b) / 3
		    
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

    return 0;
}

