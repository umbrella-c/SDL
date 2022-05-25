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

#ifdef SDL_TIMER_VALI

#include "SDL_timer.h"
#include <os/mollenos.h>
#include <threads.h>
#include <time.h>

static SDL_bool ticks_started = SDL_FALSE;

void
SDL_TicksInit(void)
{
    if (ticks_started) {
        return;
    }
    ticks_started = SDL_TRUE;
}

void
SDL_TicksQuit(void)
{
    ticks_started = SDL_FALSE;
}

Uint32
SDL_GetTicks(void)
{
	struct timespec now;
    if (!ticks_started) {
        SDL_TicksInit();
    }

	timespec_get(&now, TIME_PROCESS);
	return (uint32_t)((now.tv_sec * 1000) + (now.tv_nsec / NSEC_PER_MSEC));
}

Uint64
SDL_GetPerformanceCounter(void)
{
    LargeInteger_t value;
    if (QueryPerformanceTimer(&value) != OsSuccess) {
        return SDL_GetTicks();
    }
    return value.QuadPart;
}

Uint64
SDL_GetPerformanceFrequency(void)
{
    LargeInteger_t frequency;
    if (QueryPerformanceFrequency(&frequency) != OsSuccess) {
        return 1000;
    }
    return frequency.QuadPart;
}

void
SDL_Delay(Uint32 ms)
{
	thrd_sleepex(ms);
}

#endif // SDL_TIMER_VALI

/* vi: set ts=4 sw=4 expandtab: */
