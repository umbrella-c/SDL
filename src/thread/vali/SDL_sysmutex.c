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

#include <errno.h>
#include <threads.h>

#include "SDL_thread.h"

struct SDL_mutex
{
    mtx_t id;
};

SDL_mutex *
SDL_CreateMutex(void)
{
    SDL_mutex *mutex;

    /* Allocate the structure */
    mutex = (SDL_mutex *) SDL_calloc(1, sizeof(*mutex));
    if (mutex) {
        if (mtx_init(&mutex->id, mtx_recursive) != thrd_success) {
            SDL_SetError("mtx_init() failed");
            SDL_free(mutex);
            mutex = NULL;
        }
    } else {
        SDL_OutOfMemory();
    }
    return (mutex);
}

void
SDL_DestroyMutex(SDL_mutex * mutex)
{
    if (mutex) {
        mtx_destroy(&mutex->id);
        SDL_free(mutex);
    }
}

/* Lock the mutex */
int
SDL_LockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    if (mtx_lock(&mutex->id) != thrd_success) {
        return SDL_SetError("mtx_lock() failed");
    }
    return 0;
}

int
SDL_TryLockMutex(SDL_mutex * mutex)
{
    int retval;
    int result;
    
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    retval = 0;

    result = mtx_trylock(&mutex->id);
    if (result != thrd_success) {
        if (result == thrd_busy) {
            retval = SDL_MUTEX_TIMEDOUT;
        } else {
            retval = SDL_SetError("mtx_trylock() failed");
        }
    }
    return retval;
}

int
SDL_UnlockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    if (mtx_unlock(&mutex->id) != thrd_success) {
        return SDL_SetError("mtx_unlock() failed");
    }

    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */
