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
#include "../../SDL_internal.h"

#ifdef SDL_FILESYSTEM_VALI

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* System dependent filesystem routines                                */

#include "SDL_error.h"
#include "SDL_stdinc.h"
#include "SDL_filesystem.h"
#include <os/mollenos.h>

char *
SDL_GetBasePath(void)
{
    OsStatus_t osStatus;
    char*      path;

    path = SDL_malloc(_MAXPATH);
    if (!path) {
        return NULL;
    }

    memset(path, 0, _MAXPATH);
    osStatus = GetApplicationDirectory(&path[0], _MAXPATH);
    if (osStatus != OsSuccess) {
        SDL_free(path);
        return NULL;
    }
    strcat(path, "/");
    return path;
}

char *
SDL_GetPrefPath(const char *org, const char *app)
{
    char* path;

    path = SDL_malloc(_MAXPATH);
    if (!path) {
        return NULL;
    }

    // This should be implemented with LFS once we get there
    memset(path, 0, _MAXPATH);
    sprintf(&path[0], "$share/%s/%s/", org, app);
    return path;
}

#endif /* SDL_FILESYSTEM_VALI */

/* vi: set ts=4 sw=4 expandtab: */
