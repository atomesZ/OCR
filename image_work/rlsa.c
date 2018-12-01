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

void SDL_to_matrix(SDL_Surface *img, int* mat) {
    
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

void rlsa_l(int* mat, size_t rows, size_t cols, size_t hor_thres) {

    size_t zero_count = 0;
           
    for (size_t i = 0; i < rows; i++) {
     
        for (size_t j = 0; j < cols; j++) {

            if (mat[i * cols + j] == 1) {
                if (zero_count != 0) {
                    if (zero_count < hor_thres) {
                        for(size_t k = i - zero_count; k < i; k++) {
                            mat[k * cols + j] = 1;        
                            }
                    }
                zero_count = 0;
                }
            }
            else {
                zero_count += 1;
            }
        }
        zero_count = 0;
    }
}

void rlsa_SDL(SDL_Surface* img, int thrs) {

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

void rlsa_SDL2(SDL_Surface* img, int thrs) {
    
    int zero_count = 0;
    for (int y = 0; y < img->h; ++y) {

        zero_count = 0;
        for (int x = 0; x < img->w; ++x) {

            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if (r == 255) {

                if (zero_count < thrs) {
                    for (int z = x - zero_count; z < x; ++z) {
                        pixel = SDL_MapRGB(img->format, 0, 0, 0);
                        put_pixel(img, z, y, pixel);
                    }
                }
            }

            else {
                ++zero_count;
            }
        }
    }
}


//void rlsa_c (double mat[],size_t rows, size_t cols, size_t hor_thres)
///{
//  size_t zero_count = 0;
//    
//    for (size_t j = 0; j<cols; j++) {
//    
//       for (size_t i = 0; i<rows; i++) {
//
//            if (mat[i*cols+j] == 1) {
//
//                if (zero_count != 0) {
//                    if (zero_count < hor_thres) {
//                       for(size_t k = i - zero_count; k < i; k++) {
//                           mat[i*cols+j] = 1;        
//                        }
//                    }
//                   zero_count = 0;
//                }
//            }
//        else 
//        {
//            zero_count += 1;
//        }
//        }
//
//        zero_count = 0;
//    }
//}

//void rlsa (double matL[], double matC[], size_t rows, size_t cols, double matRes[])
//{
//    for (size_t i = 0; i<rows ; i++)
//    {
//        for (size_t j = 0; j<cols; j++)
//        {
//            if ((matL[i*cols+j] == 1)&&(matC[i*cols+j] == 1))
//            {
//                matRes[i*cols+j] = 1;
//            }
//            else
//            {
//                matRes[i*cols+j] = 0;
//            }
//        }
//    }
//}

void print_mat(int* mat, int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%i", mat[i * cols + j]);
        }
        printf("\n");
    }
}

//void line (double mat[], size_t rows, size_t cols, size_t i, size_t j, size_t i2, size_t j2)
//{
//    while (mat[i * cols + j] == 0)
//    {
//        if (j < cols)
//        {
//            j+=1;
//        }
//        else 
//        {
//            i+=1;
//            j=0;
//        }
//    }
//    
//    j2 = j;
//    while ((j2 < cols)&&(mat[i* cols + j2] == 1))
//    {
//       j2+=1;
//    }
//    i2 = i;
//    while ((i2 < rows)&&(mat[i2* cols + j2] == 1))
//    {
//        i2+=1;
//    }
//}

//void reduc (double mat[], double matres[], size_t cols, size_t i, size_t i2, size_t j, size_t j2)
//{
//    for (size_t k = i; k<=i2; k++)
//    {
//        for(size_t l = j; l<=j2; l++)
//       {
//            matres[(k-i)*(j2-j)+(l-j)] = mat[k*cols+l];
//        }
//    }
//}

int main() {
    
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // Initialize the SDL
    init_sdl();

    // Load the image
    image_surface = load_image("image_test/my_blackandwhite_image.bmp");

    // Matrix
    int *mat;
    mat = malloc(sizeof(int) * image_surface->h * image_surface->w);

    size_t rows = image_surface->h;
    size_t cols = image_surface->w;

    // Display the image
    screen_surface = display_image(image_surface);
    
    wait_for_keypressed();

    SDL_to_matrix(image_surface, mat);
    print_mat(mat, rows, cols);
    printf("---------------------------------------------------------------------------------------\n");
    rlsa_SDL2(image_surface, 100);
    display_image(image_surface);

    free(mat);

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
