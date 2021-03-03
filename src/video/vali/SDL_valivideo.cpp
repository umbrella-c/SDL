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

#define SDL_VIDEO_DRIVER_VALI 1
#if SDL_VIDEO_DRIVER_VALI

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_valivideo.h"
#include "SDL_valievents.h"
#include "SDL_valiframebuffer.h"
#include "SDL_vali_window.h"

#define VALIVID_DRIVER_NAME "vali"

/* Initialization/Query functions */
static int  VALI_VideoInit(_THIS);
static int  VALI_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void VALI_VideoQuit(_THIS);

extern SdlWindow* m_window;

static void
VALI_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_free(device);
}

static SDL_VideoDevice *
VALI_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return (0);
    }

    /* Set the function pointers */
    device->VideoInit = VALI_VideoInit;
    device->VideoQuit = VALI_VideoQuit;
    device->SetDisplayMode = VALI_SetDisplayMode;
    device->PumpEvents = VALI_PumpEvents;
    device->CreateSDLWindow = SDL_VALI_CreateWindow;
    device->DestroyWindow = SDL_VALI_DestroyWindow;
    device->CreateWindowFramebuffer = SDL_VALI_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = SDL_VALI_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = SDL_VALI_DestroyWindowFramebuffer;

    device->free = VALI_DeleteDevice;

    return device;
}

VideoBootStrap VALI_bootstrap = {
    VALIVID_DRIVER_NAME, "SDL vali video driver", VALI_CreateDevice
};


int
VALI_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    mode.format = SDL_PIXELFORMAT_RGB888;

    mode.w = m_window->GetScreen()->GetCurrentWidth();
    mode.h = m_window->GetScreen()->GetCurrentHeight();
    mode.refresh_rate = m_window->GetScreen()->GetCurrentRefreshRate();
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }
    
    for (const& mode : m_window->GetScreen()->GetModes()) {
        SDL_zero(mode);

        mode.format = SDL_PIXELFORMAT_RGB888;
        mode.w = mode->ResolutionX();
        mode.h = mode->ResolutionY();
        mode.refresh_rate = mode->RefreshRate();
        mode.driverdata = NULL;

        SDL_AddDisplayMode(&_this->displays[0], &mode);

    }
    return 0;
}

static int
VALI_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{
    return 0;
}

void
VALI_VideoQuit(_THIS)
{
    // nothing to do
}

#endif /* SDL_VIDEO_DRIVER_DUMMY */

/* vi: set ts=4 sw=4 expandtab: */
