#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "interface.h"

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

int wait_for_keypressed()
{
    SDL_Event event;
    int continuer = 1;

    while (continuer) {
        
        SDL_WaitEvent(&event);
        SDL_PollEvent(&event);

        switch(event.type) {

            case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {

                case SDLK_ESCAPE:
                return 1;
                break;

                default:
		return 0;
                break;
            }
        }
    }

    return 0;
        
    
    // Wait for a key to be down.

    /* do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN && event.key.keysym.sym != SDLK_ESCAPE);

    // Wait for a key to be up.

    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP && event.key.keysym.sym != SDLK_ESCAPE); */

}

void display(char *file)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // Initialize the SDL

    init_sdl();

    image_surface = load_image(file);
   
    // Display the image.
    screen_surface = display_image(image_surface);

    // Wait for a keystroke
    int boolean = wait_for_keypressed();

    if (boolean)
	SDL_Quit();
    
    else {
        
        // Free the image surface.
        SDL_FreeSurface(image_surface);

        // Free the screen surface
        SDL_FreeSurface(screen_surface);
    
        // Return to the main interface menu
        interface();
    }
}

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

    // Wait for a keystroke
    int boolean = wait_for_keypressed();

    if (boolean)
	SDL_Quit();
    
    else {
        
        // Free the image surface.
        SDL_FreeSurface(image_surface);

        // Free the screen surface
        SDL_FreeSurface(screen_surface);
    
        // Return to the main interface menu
        interface();
    }
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

    // Save the image to "myblackandwhite_image.bmp"
    SDL_SaveBMP(image_surface, "image_test/my_blackandwhite_image.bmp");

    // Update the surfaces
    update_surface(screen_surface, image_surface);

    // Wait for a keystroke
    int boolean = wait_for_keypressed();

    if (boolean)
	SDL_Quit();
    
    else {
        
        // Free the image surface.
        SDL_FreeSurface(image_surface);

        // Free the screen surface
        SDL_FreeSurface(screen_surface);
    
        // Return to the main interface menu
        interface();
    }
}


