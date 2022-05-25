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

#include "../SDL_sysvideo.h"
#include "SDL_vioarrframebuffer.h"
#include "SDL_vioarr_window.h"

int SDL_VALI_CreateWindowFramebuffer(_THIS, SDL_Window * window, Uint32 * format, void ** pixels, int *pitch)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return -1;
    }

    // Create a new window buffer
    if (sdlWindow->GetBuffer()) {
        sdlWindow->DeleteWindowBuffer();
    }

    sdlWindow->CreateWindowBuffer(Asgaard::PixelFormat::X8B8G8R8, Asgaard::MemoryBuffer::Flags::NONE);

    *format = SDL_PIXELFORMAT_ABGR8888;
    *pixels = sdlWindow->GetBuffer()->Buffer();
    *pitch = sdlWindow->GetBuffer()->Stride();
    return 0;
}

int SDL_VALI_UpdateWindowFramebuffer(_THIS, SDL_Window * window, const SDL_Rect * rects, int numrects)
{
    auto sdlWindow = (SdlWindow*)SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return -1;
    }

    sdlWindow->RequestRedraw();
    return 0;
}

void SDL_VALI_DestroyWindowFramebuffer(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*)SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->DeleteWindowBuffer();
}

#endif /* SDL_VIDEO_DRIVER_VIOARR */

/* vi: set ts=4 sw=4 expandtab: */
