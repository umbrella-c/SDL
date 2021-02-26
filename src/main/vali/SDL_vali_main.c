/*
    SDL_vali_main.c, placed in the public domain by Sam Lantinga  4/13/98
*/
#include "SDL_config.h"

#ifdef __VALI__

/* Include this so we define UNICODE properly */
#include "../../core/vali/SDL_vali.h"

/* Include the SDL main definition header */
#include "SDL.h"
#include "SDL_main.h"

#ifdef main
#  undef main
#endif /* main */

int main(int argc, char *argv[])
{
    SDL_SetMainReady();
    result = SDL_main(argc, argv);
    return result;
}

#endif /* __VALI__ */

/* vi: set ts=4 sw=4 expandtab: */
