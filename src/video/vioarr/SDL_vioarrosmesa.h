/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2021 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SDL_vioarrosmesa_h_
#define SDL_vioarrosmesa_h_

#include "../../SDL_internal.h"

#include "SDL_vioarrvideo.h"


extern int           VALI_OSGL_LoadLibrary(_THIS, const char *path);
extern void*         VALI_OSGL_GetProcAddress(_THIS, const char *proc);
extern void          VALI_OSGL_UnloadLibrary(_THIS);
extern SDL_GLContext VALI_OSGL_CreateContext(_THIS, SDL_Window * window);
extern int           VALI_OSGL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern int           VALI_OSGL_SetSwapInterval(_THIS, int interval);
extern int           VALI_OSGL_GetSwapInterval(_THIS);
extern int           VALI_OSGL_SwapWindow(_THIS, SDL_Window * window);
extern void          VALI_OSGL_DeleteContext(_THIS, SDL_GLContext context);
extern void          VALI_OSGL_DefaultProfileConfig(_THIS, int *mask, int *major, int *minor);

#endif /* SDL_vioarrosmesa_h_ */

/* vi: set ts=4 sw=4 expandtab: */
