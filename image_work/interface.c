#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "err.h"
#include "display.h"
#include "rlsa.h"
#include "network.h"
#include "backprop.h"
#include "interface.h"

// Loop that stop when the press the "x" button
void pause1()
{
    Uint32 rmask, gmask, bmask, amask;
    
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    
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

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    
                    case SDLK_ESCAPE:
                    continuer = 0;
		    SDL_Quit();
                    break;

		    default:
		    continuer = 0;
		    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                X = event.button.x;
                Y = event.button.y;
                
                // Display button 
                if (X > 206 && Y > 260 && X < 583 && Y < 338) {
                    display("image_test/my_image.png");
                    continuer = 0;
                }
                
                // Grayscale button
                else if (X > 206 && Y > 340 && X < 583 && Y < 416) {
                    grayscale("image_test/my_image.png");
		            continuer = 0;
		        }

                // Black & White button
                else if (X > 206 && Y > 417 && X < 583 && Y < 495) {
                    whiteandblack("image_test/my_image.png");
		            continuer = 0;
		        }

                // Text Segmentation button
                else if (X > 206 && Y > 496 && X < 583 && Y < 573) {
                     rlsa("image_test/my_image.png");
                     continuer = 0;
                }

                // Text Recognition button
                else if (X > 206 && Y > 573 && X < 583 && Y < 652) {
		    SDL_Surface *tim = IMG_Load("image_test/tim.png");
                    SDL_Surface *tim_resize;
		    tim_resize = tim;
		    if(tim->h != 32 && tim->w != 32)
		    {
                    	tim_resize = SDL_CreateRGBSurface(0, 32, 32, 32, rmask, gmask, bmask, amask);
                    
		    	SDL_SoftStretch(tim, NULL, tim_resize, NULL);
		    }
                    double *m;
		    m = malloc((sizeof(double)*32*32));
		    SDL_to_matrix(tim_resize, m);
		    create_dataset("timtxt", m, 42);
		    //print_mat(m,32,32);
		    free(m);
		    Network net = loadNetwork("saybe");
 
		    char *s;
		    int num_char = 1;
		    s = malloc(sizeof(char) * num_char);
		    readtxt(net, "timtxt", s, num_char);
		    printf(s);
		    printf("\n");
		    free(s);
		    
		 }
                             
                break;           
        }
    }
}

void interface() 
{
    //Network net = loadNetwork("saybe");
    /*char *s;
    int num_char = 946;
    s = malloc(sizeof(char) * num_char);
    readtxt(net, "data_set", s, num_char);
    printf(s);
    free(s);*/
    // Image of the interface
    SDL_Surface *interface = IMG_Load("interface.png");

    // Property of the window
    SDL_Surface *screen = SDL_SetVideoMode(interface->w, interface->h, 32, SDL_SWSURFACE|SDL_ANYFORMAT);

    // Blit interface onto the screen
    SDL_BlitSurface(interface, NULL, screen, NULL);

    SDL_UpdateRect(screen, 0, 0, interface->w, interface->h);

    // Title
    SDL_WM_SetCaption("OCR", NULL);
    
    pause1();
    SDL_Quit();
}
