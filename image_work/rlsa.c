#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "interface.h"
#include "rlsa.h"

void SDL_to_matrix(SDL_Surface *img, double* mat) {
    
    for(int x = 0; x < img->h; ++x) {
        for(int y = 0; y < img->w; ++y) {
            Uint32 pixel = get_pixel(img, y, x);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if (r == 0)
                mat[x * img->h + y] = 1;
            else
                mat[x * img->h + y] = 0;
            }
    }
}

void create_dataset(char* filename, double *m, char value)
{
    FILE *file;//declare file
    file = fopen(filename, "w+");//create file or add

    int dim = 32;

    if(file == NULL)
        errx(1, "Could not open file");

    for(int h = 0; h < dim; ++h)
    {
        for(int j = 0; j < dim; ++j)
        {
            fputc((int)m[h*dim + j] + 48, file);
        }
        fputc(10, file);
    }
    fputc((int)value, file);
    fputc(10, file);
    fclose(file);
}

void rlsa_SDLup(SDL_Surface* img, int thrs) {

    int zero_count = 0;
    for (int x = 0; x < img->w; ++x) {

        zero_count = 0;
        for (int y = 0; y < img->h; ++y) {

            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if (r == 255) {

                if (zero_count < thrs) {
                    for (int z = y - zero_count; z < y; ++z) {
                        pixel = SDL_MapRGB(img->format, 0, 0, 0);
                        put_pixel(img, x, z, pixel);
                    }
                }
            }

            else {
                ++zero_count;
            }
        }
    }
}

void rlsa_SDLdown(SDL_Surface* img, int thrs) {

    // Zero counter
    int zero_count = 0;
    for (int x = 0; x < img->w; ++x) {

        // Reset counter because moving to next column
        zero_count = 0;
        for (int y = img->h - 1; y > 0; --y) {

            // Recovering the pixel
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Black pixel case
            if (r == 255) {

                // Zero counter under threshold
                if (zero_count < thrs) {
                    for (int z = y + zero_count; z > y; --z) {
                        pixel = SDL_MapRGB(img->format, 0, 0, 0);
                        put_pixel(img, x, z, pixel);
                    }
                }
            }

            // White pixel case
            else {
                ++zero_count;
            }
        }
    }
}

void rlsa_SDLleft(SDL_Surface* img, int thrs) {
   
    // Zero counter
    int zero_count = 0;
    for (int y = 0; y < img->h; ++y) {

        // Reset counter because moving to next line 
        zero_count = 0;
        for (int x = 0; x < img->w; ++x) {
            
            // Recovering the pixel 
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Black pixel case
            if (r == 255) {
                
                // Zero counter under threshold
                if (zero_count < thrs) {
                    for (int z = x - zero_count; z < x; ++z) {
                        pixel = SDL_MapRGB(img->format, 0, 0, 0);
                        put_pixel(img, z, y, pixel);
                    }
                }

                else
                    zero_count = 0;
            }

            // White pixel case
            else {
                ++zero_count;
            }
        }
    }
}


void rlsa_SDLright(SDL_Surface* img, int thrs) {
   
    // Zero counter
    int zero_count = 0;
    for (int y = 0; y < img->h; ++y) {

        // Reset counter because moving to next column
        zero_count = 0;
        for (int x = img->w - 1; x > 0; --x) {
            
            // Recovering the pixel
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Black pixel case
            if (r == 255) {
                
                // Zero counter under threshold
                if (zero_count < thrs) {
                    for (int z = x + zero_count; z > x; --z) {
                        pixel = SDL_MapRGB(img->format, 0, 0, 0);
                        put_pixel(img, z, y, pixel);
                    }
                }

                else
                    zero_count = 0;
            }

            // White pixel case
            else {
                ++zero_count;
            }
        }
    }
}

void rlsalc(SDL_Surface* img1, SDL_Surface* img2, SDL_Surface* imgr) {

    for (int x = 0; x < img1->w; ++x) {
        for (int y = 0; y < img2->h; ++y) {
            Uint32 pixel1 = get_pixel(img1, x, y);
            Uint8 r1, g1, b1;
            SDL_GetRGB(pixel1, img1->format, &r1, &g1, &b1);
 
            Uint32 pixel2 = get_pixel(img2, x, y);
            Uint8 r2, g2, b2;
            SDL_GetRGB(pixel2, img2->format, &r2, &g2, &b2);

            if ((r1 == 0) && (r2 == 0)) {
                Uint32 pixel = SDL_MapRGB(imgr->format, 0, 0, 0);
                put_pixel(imgr, x, y, pixel);
            }

            else {
                Uint32 pixel = SDL_MapRGB(imgr->format, 255, 255, 255);
                put_pixel(imgr, x, y, pixel);
            }
        }
    }
}

void print_mat(double* mat, int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%i",(int) mat[i * cols + j]);
        }
        printf("\n");
    }
}


void rlsa(char* file) {
    
    SDL_Surface* image_surface1;
    SDL_Surface* image_surface2;
    SDL_Surface* image_surface3;
    SDL_Surface* image_surface4;
    SDL_Surface* result1; 
    SDL_Surface* result2;
    SDL_Surface* result;

    // Pixels masks
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

    // Load the image
    image_surface1 = load_image(file);
    image_surface2 = load_image(file);
    image_surface3 = load_image(file);
    image_surface4 = load_image(file);

    result1 = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);

    result2 = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);

    result = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);


    rlsa_SDLup(image_surface1, 1600);
    // display_image(image_surface1);
    // wait_for_keypressed();

    rlsa_SDLdown(image_surface2, 1600);
    // display_image(image_surface2);
    // wait_for_keypressed();

    rlsalc(image_surface1, image_surface2, result1);
    // display_image(result1);
    // wait_for_keypressed();

    rlsa_SDLleft(image_surface3, 1000);
    // display_image(image_surface3);
    // wait_for_keypressed();

    rlsa_SDLright(image_surface4, 1000);
    // display_image(image_surface4);
    // wait_for_keypressed();

    rlsalc(image_surface3, image_surface4, result2);
    // display_image(result2);
    // wait_for_keypressed();

    rlsalc(result1, result2, result);
    display_image(result);
    // wait_for_keypressed();

    int boolean = wait_for_keypressed();

    if (boolean)
        SDL_Quit();
    
    else {
        // Free the image surface.   
        SDL_FreeSurface(image_surface1);
        SDL_FreeSurface(image_surface2);
        SDL_FreeSurface(result1);
        SDL_FreeSurface(image_surface3);
        SDL_FreeSurface(image_surface4);
        SDL_FreeSurface(result2);
        SDL_FreeSurface(result);

        interface();
    }
}
