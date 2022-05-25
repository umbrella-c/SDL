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
#include "../SDL_sysvideo.h"

#include "SDL_mouse.h"
#include "../../events/SDL_mouse_c.h"
#include "SDL_vioarrvideo.h"
}

#if 0

typedef struct {
    struct wl_buffer   *buffer;
    struct wl_surface  *surface;

    int                hot_x, hot_y;
    int                w, h;

    /* Either a preloaded cursor, or one we created ourselves */
    struct wl_cursor   *cursor;
    void               *shm_data;
} Wayland_CursorData;

static SDL_Cursor *
VALI_CreateDefaultCursor()
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();
    SDL_VideoData *data = device->driverdata;

    return CreateCursorFromWlCursor (data,
                                     WAYLAND_wl_cursor_theme_get_cursor(data->cursor_theme,
                                                                "left_ptr"));
}

static SDL_Cursor *
VALI_CreateSystemCursor(SDL_SystemCursor id)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;

    struct wl_cursor *cursor = NULL;

    switch(id)
    {
    default:
        SDL_assert(0);
        return NULL;
    case SDL_SYSTEM_CURSOR_ARROW:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "left_ptr");
        break;
    case SDL_SYSTEM_CURSOR_IBEAM:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "xterm");
        break;
    case SDL_SYSTEM_CURSOR_WAIT:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "watch");
        break;
    case SDL_SYSTEM_CURSOR_CROSSHAIR:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_WAITARROW:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "watch");
        break;
    case SDL_SYSTEM_CURSOR_SIZENWSE:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_SIZENESW:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_SIZEWE:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_SIZENS:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_SIZEALL:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    case SDL_SYSTEM_CURSOR_NO:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "xterm");
        break;
    case SDL_SYSTEM_CURSOR_HAND:
        cursor = WAYLAND_wl_cursor_theme_get_cursor(d->cursor_theme, "hand1");
        break;
    }

    return CreateCursorFromWlCursor(d, cursor);
}

static void
VALI_FreeCursor(SDL_Cursor *cursor)
{
    Wayland_CursorData *d;

    if (!cursor)
        return;

    d = cursor->driverdata;

    /* Probably not a cursor we own */
    if (!d)
        return;

    if (d->buffer && !d->cursor)
        wl_buffer_destroy(d->buffer);

    if (d->surface)
        wl_surface_destroy(d->surface);

    /* Not sure what's meant to happen to shm_data */
    free (cursor->driverdata);
    SDL_free(cursor);
}

static int
VALI_ShowCursor(SDL_Cursor *cursor)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;

    struct wl_pointer *pointer = d->pointer;

    if (!pointer)
        return -1;

    if (cursor)
    {
        Wayland_CursorData *data = cursor->driverdata;

        wl_pointer_set_cursor (pointer, 0,
                               data->surface,
                               data->hot_x,
                               data->hot_y);
        wl_surface_attach(data->surface, data->buffer, 0, 0);
        wl_surface_damage(data->surface, 0, 0, data->w, data->h);
        wl_surface_commit(data->surface);
    }
    else
    {
        wl_pointer_set_cursor (pointer, 0,
                               NULL,
                               0,
                               0);
    }
    
    return 0;
}

static void
VALI_WarpMouse(SDL_Window *window, int x, int y)
{
    SDL_Unsupported();
}

static int
VALI_WarpMouseGlobal(int x, int y)
{
    return SDL_Unsupported();
}

static int
VALI_SetRelativeMouseMode(SDL_bool enabled)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *data = (SDL_VideoData *) vd->driverdata;

    if (enabled)
        return Wayland_input_lock_pointer(data->input);
    else
        return Wayland_input_unlock_pointer(data->input);
}
#endif

void
VALI_InitMouse(void)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    //mouse->CreateCursor = VALI_CreateCursor;
    //mouse->CreateSystemCursor = VALI_CreateSystemCursor;
    //mouse->ShowCursor = VALI_ShowCursor;
    //mouse->FreeCursor = VALI_FreeCursor;
    //mouse->WarpMouse = VALI_WarpMouse;
    //mouse->WarpMouseGlobal = VALI_WarpMouseGlobal;
    //mouse->SetRelativeMouseMode = VALI_SetRelativeMouseMode;

    //SDL_SetDefaultCursor(VALI_CreateDefaultCursor());
}

void
VALI_FiniMouse(void)
{
    /* This effectively assumes that nobody else
     * touches SDL_Mouse which is effectively
     * a singleton */
}
#endif  /* SDL_VIDEO_DRIVER_VIOARR */
