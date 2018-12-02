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

/* void rlsa_l(int* mat, size_t rows, size_t cols, size_t hor_thres) {

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
}*/

void rlsa_SDLup(SDL_Surface* img, int thrs) {

    int zero_count = 0;
    for (int x = 0; x < img->w - 1; ++x) {

        zero_count = 0;
        for (int y = 0; y < img->h - 1; ++y) {

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
    for (int x = 0; x < img->w - 1; ++x) {

        // Reset counter because moving to next column
        zero_count = 0;
        for (int y = img->h - 1; y > 1; --y) {

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
    for (int y = 0; y < img->h - 1; ++y) {

        // Reset counter because moving to next line 
        zero_count = 0;
        for (int x = 0; x < img->w - 1; ++x) {
            
            // Recovering the pixel 
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Black pixel case
            if (r == 0) {
                
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
    for (int y = 0; y < img->h - 1; ++y) {

        // Reset counter because moving to next column
        zero_count = 0;
        for (int x = img->w - 1; x > 1; --x) {
            
            // Recovering the pixel
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Black pixel case
            if (r == 0) {
                
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

void rlsa(SDL_Surface* img1, SDL_Surface* img2, SDL_Surface* imgr) {

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

void resizeL (SDL_Surface* img1, SDL_Surface* imgr, float pas)
{
    float res = 0;
    float test = 0;
    for (int x = 0; x<imgr -> h; ++x)
    {
        for (int y = 0; y<imgr->w; ++y)
        {
            res= y*pas;
            if (res < img1->w)
            {
                test = res;
                while (test >= 1)
                {
                     test -=1;
                }
                if (test == 0)
                {
                    Uint32 pixel1 = get_pixel(img1, x, res);
                    Uint8 r1, g1, b1;
                    SDL_GetRGB(pixel1, img1->format, &r1, &g1, &b1);

                    Uint32 pixel = SDL_MapRGB(img1->format, r1, r1, r1);
                    put_pixel(imgr, x, y, pixel);

                }
                else
                {
                    Uint32 pixel1 = get_pixel(img1, x, (res-test));
                    Uint8 r1, g1, b1;
                    SDL_GetRGB(pixel1, img1->format, &r1, &g1, &b1);

                    Uint32 pixel2 = get_pixel(img1, x, (res+(1-test)));
                    Uint8 r2, g2, b2;
                    SDL_GetRGB(pixel2, img1->format, &r2, &g2, &b2);

                    float r = r1*(1-test) + r2*test;
                    Uint32 pixel = SDL_MapRGB(imgr->format, r, r, r);
                    put_pixel(imgr, x, y, pixel);
                }
            }
            else 
            {}
        }
    }
}

void resizeC (SDL_Surface* img1, SDL_Surface* imgr, float pas)
{
    float res = 0;
    float test = 0;
    for (int y = 0; y<imgr -> w; ++y)
    {
        for (int x = 0; x<imgr->h; ++x)
        {
            
            res= x*pas;
            test = res;
            while (test >= 1)
            {
                test -=1;
            }
            if (test == 0)
            {
                Uint32 pixel1 = get_pixel(img1, res, y);
                       Uint8 r1, g1, b1;
                       SDL_GetRGB(pixel1, img1->format, &r1, &g1, &b1);

                Uint32 pixel = SDL_MapRGB(img1->format, r1, r1, r1);
                            put_pixel(imgr, x, y, pixel);

            }
            else
            {
                Uint32 pixel1 = get_pixel(img1, (res-test), y);
                        Uint8 r1, g1, b1;
                        SDL_GetRGB(pixel1, imgr->format, &r1, &g1, &b1);
            
                Uint32 pixel2 = get_pixel(img1, (res+(1-test)), y);
                       Uint8 r2, g2, b2;
                       SDL_GetRGB(pixel2, img1->format, &r2, &g2, &b2);

                float r = r1*(1-test) + r2*test;
                Uint32 pixel = SDL_MapRGB(imgr->format, r, r, r);
                            put_pixel(imgr, x, y, pixel);

            }
        }
    }
}

int main() {
    
    SDL_Surface* image_surface1;
    SDL_Surface* image_surface2;
    SDL_Surface* image_surface3;
    SDL_Surface* image_surface4;
    SDL_Surface* result1; 
    SDL_Surface* result2;
    SDL_Surface* result;

    //SDL_Surface* screen_surface;

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

    // Initialize the SDL
    init_sdl();

    // Load the image
    image_surface1 = load_image("image_test/my_blackandwhite_image.bmp");
    image_surface2 = load_image("image_test/my_blackandwhite_image.bmp");
    image_surface3 = load_image("image_test/my_blackandwhite_image.bmp");
    image_surface4 = load_image("image_test/my_blackandwhite_image.bmp");

    result1 = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);

    result2 = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);

    result = SDL_CreateRGBSurface(0, image_surface1->w, image_surface1->h, 32,
                                        rmask, gmask, bmask, amask);
    //SDL_SoftStretch(image_surface, NULL, result, NULL);

    // Matrix
    //int *mat;
    //mat = malloc(sizeof(int) * result->h * result->w);

    //size_t rows = result->h;
    //size_t cols = result->w;

    // Display the image
    //screen_surface = display_image(image_surface1);
    //wait_for_keypressed();

    //SDL_to_matrix(result, mat);
    //print_mat(mat, rows, cols);
    //printf("---------------------------------------------------------------------------------------\n");
    
    SDL_Surface* r;
    r = load_image("image_test/caractere.png");
    display_image(r);
    wait_for_keypressed();

    SDL_Surface* res;
    res = SDL_CreateRGBSurface(0,32,32,32,rmask,gmask,bmask, amask);
    SDL_SoftStretch (r,NULL,res,NULL);

    display_image(res);
    wait_for_keypressed();

    rlsa_SDLup(image_surface1, 1000);
    display_image(image_surface1);
    wait_for_keypressed();

    rlsa_SDLdown(image_surface2, 1000);
    display_image(image_surface2);
    wait_for_keypressed();

    rlsa(image_surface1, image_surface2, result1);
    display_image(result1);
    wait_for_keypressed();

    rlsa_SDLleft(image_surface3, 400);
    display_image(image_surface3);
    wait_for_keypressed();

    rlsa_SDLright(image_surface4, 400);
    display_image(image_surface4);
    wait_for_keypressed();

    rlsa(image_surface3, image_surface4, result2);
    display_image(result2);
    wait_for_keypressed();

    rlsa(result1, result2, result);
    display_image(result);
    wait_for_keypressed();

    rlsa_SDLleft(result, 23);
    display_image(result);
    wait_for_keypressed();

    rlsa_SDLright(result, 23);
    display_image(result);

    

    //free(mat);

    // Update the surfaces    
    //update_surface(screen_surface, image_surface);
    
    // Wait for a key to be pressed.
    wait_for_keypressed();

    // Free the image surface.   
    SDL_FreeSurface(image_surface1);
    SDL_FreeSurface(image_surface2);
    SDL_FreeSurface(result1);
    SDL_FreeSurface(image_surface3);
    SDL_FreeSurface(image_surface4);
    SDL_FreeSurface(result2);
    SDL_FreeSurface(result);

    // Free the screen surface.
    //SDL_FreeSurface(screen_surface);

    return 0;
}
