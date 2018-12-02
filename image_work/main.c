#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "err.h"
#include "display.h"
#include "interface.h"
#include "rlsa.h"

int main() 
{
    SDL_Init(SDL_INIT_VIDEO);
    interface();
    return 0;
}
