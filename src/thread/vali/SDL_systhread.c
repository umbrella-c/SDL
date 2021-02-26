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

#include "SDL_system.h"
#include "SDL_hints.h"

#include <os/mollenos.h>
#include <signal.h>

#include "SDL_platform.h"
#include "SDL_thread.h"
#include "../SDL_thread_c.h"
#include "../SDL_systhread.h"

static int
RunThread(void *data)
{
    SDL_RunThread((SDL_Thread *)data);
    return 0;
}

int
SDL_SYS_CreateThread(SDL_Thread * thread)
{
    // todo support for stacksize arg

    if (thrd_create(&thread->handle, (thrd_start_t)RunThread, thread) != thrd_success) {
        return SDL_SetError("Not enough resources to create thread");
    }
    return 0;
}

void
SDL_SYS_SetupThread(const char *name)
{
    if (name != NULL) {
        SetCurrentThreadName(name);
    }
}

SDL_threadID
SDL_ThreadID(void)
{
    return ((SDL_threadID) thrd_current());
}

int
SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
    return SDL_Unsupported();
}

void
SDL_SYS_WaitThread(SDL_Thread * thread)
{
    thrd_join(thread->handle, 0);
}

void
SDL_SYS_DetachThread(SDL_Thread * thread)
{
    thrd_detach(thread->handle);
}

/* vi: set ts=4 sw=4 expandtab: */
