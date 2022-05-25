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

#if SDL_VIDEO_DRIVER_VIOARR

extern "C" {
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
}

#include "SDL_vioarrvideo.h"
#include "SDL_vioarrevents.h"
#include "SDL_vioarrframebuffer.h"
#include "SDL_vioarr_window.h"
#include "SDL_vioarrosmesa.h"
#include "SDL_vioarrmouse.h"

#define VALIVID_DRIVER_NAME "vioarr"

/* Initialization/Query functions */
static int  VALI_VideoInit(_THIS);
static int  VALI_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void VALI_VideoQuit(_THIS);

static int g_appReferences = 0;

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

    // essential implementations
    device->VideoInit                = VALI_VideoInit;
    device->VideoQuit                = VALI_VideoQuit;
    device->free                     = VALI_DeleteDevice;
    device->SetDisplayMode           = VALI_SetDisplayMode;
    device->PumpEvents               = VALI_PumpEvents;
    device->CreateSDLWindow          = SDL_VALI_CreateWindow;
    device->DestroyWindow            = SDL_VALI_DestroyWindow;
    device->CreateWindowFramebuffer  = SDL_VALI_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer  = SDL_VALI_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = SDL_VALI_DestroyWindowFramebuffer;

    // window functionality
    device->SetWindowTitle       = SDL_VALI_SetWindowTitle;
    device->SetWindowIcon        = SDL_VALI_SetWindowIcon;
    device->SetWindowPosition    = SDL_VALI_SetWindowPosition;
    device->SetWindowSize        = SDL_VALI_SetWindowSize;
    device->GetWindowBordersSize = SDL_VALI_GetWindowBordersSize;
    device->ShowWindow           = SDL_VALI_ShowWindow;
    device->HideWindow           = SDL_VALI_HideWindow;
    device->RaiseWindow          = SDL_VALI_RaiseWindow;
    device->MaximizeWindow       = SDL_VALI_MaximizeWindow;
    device->MinimizeWindow       = SDL_VALI_MinimizeWindow;
    device->RestoreWindow        = SDL_VALI_RestoreWindow;
    device->SetWindowBordered    = SDL_VALI_SetWindowBordered;
    device->SetWindowResizable   = SDL_VALI_SetWindowResizable;
    device->SetWindowFullscreen  = SDL_VALI_SetWindowFullscreen;
    device->SetWindowMouseGrab   = SDL_VALI_SetWindowMouseGrab;

    // graphics
#if SDL_VIDEO_OPENGL_OSMESA
    device->GL_LoadLibrary          = VALI_OSGL_LoadLibrary;
    device->GL_GetProcAddress       = VALI_OSGL_GetProcAddress;
    device->GL_UnloadLibrary        = VALI_OSGL_UnloadLibrary;
    device->GL_CreateContext        = VALI_OSGL_CreateContext;
    device->GL_MakeCurrent          = VALI_OSGL_MakeCurrent;
    device->GL_SetSwapInterval      = VALI_OSGL_SetSwapInterval;
    device->GL_GetSwapInterval      = VALI_OSGL_GetSwapInterval;
    device->GL_SwapWindow           = VALI_OSGL_SwapWindow;
    device->GL_DeleteContext        = VALI_OSGL_DeleteContext;
    device->GL_DefaultProfileConfig = VALI_OSGL_DefaultProfileConfig;
#endif

    return device;
}

VideoBootStrap VALI_bootstrap = {
    VALIVID_DRIVER_NAME, "SDL vioarr video driver", VALI_CreateDevice
};

void InitializeVioarr()
{
    std::string baseSdlGuid("f64cc342-868e-4b68-a0a8-eea485f402c4");
    if (Asgaard::APP.GetSettingString(Asgaard::Application::Settings::APPLICATION_GUID) == "") {
        Asgaard::APP.SetSettingString(Asgaard::Application::Settings::APPLICATION_GUID, baseSdlGuid);
    }
    Asgaard::APP.Initialize();
}

int
VALI_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    if (!g_appReferences) {
        InitializeVioarr();
    }
    g_appReferences++;

    mode.format = SDL_PIXELFORMAT_RGB888;

    mode.w = Asgaard::APP.GetScreen()->GetCurrentWidth();
    mode.h = Asgaard::APP.GetScreen()->GetCurrentHeight();
    mode.refresh_rate = Asgaard::APP.GetScreen()->GetCurrentRefreshRate();
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }
    
    for (const auto& screenMode : Asgaard::APP.GetScreen()->GetModes()) {
        SDL_zero(mode);

        mode.format = SDL_PIXELFORMAT_RGB888;
        mode.w = screenMode->ResolutionX();
        mode.h = screenMode->ResolutionY();
        mode.refresh_rate = screenMode->RefreshRate();
        mode.driverdata = NULL;

        SDL_AddDisplayMode(&_this->displays[0], &mode);

    }

    // initialize subsystems
    VALI_InitMouse();
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
    VALI_FiniMouse();
}

#endif /* SDL_VIDEO_DRIVER_DUMMY */

/* vi: set ts=4 sw=4 expandtab: */
