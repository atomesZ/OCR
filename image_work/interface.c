#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "err.h"
#include "display.h"

// Loop that stop when the press the "x" button
void pause()
{
    int continuer = 1;

    // Coordinate of the cursor
    int X = 0;
    int Y = 0;

    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);

        switch(event.type) {

            // Exit the loop
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_MOUSEBUTTONDOWN:
                X = event.button.x;
                Y = event.button.y;
                
                // Display button 
                if (X > 206 && Y > 260 && X < 583 && Y < 338) 
                    display("image_test/my_image.jpg");
                
                // Grayscale button
                if (X > 206 && Y > 340 && X < 583 && Y < 416)
                    grayscale("image_test/my_image.jpg");

                // Black & White button
                if (X > 206 && Y > 417 && X < 583 && Y < 495)

                // Text Segmentation button
                if (X > 206 && Y > 496 && X < 583 && Y < 573)

                // Text Recognition button
                if (X > 206 && Y > 573 && X < 583 && Y < 652)
                
                
                break;           
        }
    }
}

int main() 
{
    SDL_Init(SDL_INIT_VIDEO);

    // Image of the interface
    SDL_Surface *interface = IMG_Load("interface.png");

    // Property of the window
    SDL_Surface *screen = SDL_SetVideoMode(interface->w, interface->h, 32, SDL_SWSURFACE|SDL_ANYFORMAT);

    // Blit interface onto the screen
    SDL_BlitSurface(interface, NULL, screen, NULL);

    SDL_UpdateRect(screen, 0, 0, interface->w, interface->h);

    // Title
    SDL_WM_SetCaption("OCR", NULL);
    
    pause();
    SDL_Quit();

    return 0;
}
