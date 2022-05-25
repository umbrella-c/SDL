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

#ifndef SDL_core_vioarr_window_h_
#define SDL_core_vioarr_window_h_

/**
 * Copyright 2020, Philip Meulengracht
 *
 * This program is free software : you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation ? , either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * SDL - Application Window
 */
#pragma once

#include <asgaard/application.hpp>
#include <asgaard/window_base.hpp>
#include <asgaard/window_decoration.hpp>
#include <asgaard/subsurface.hpp>
#include <asgaard/memory_pool.hpp>
#include <asgaard/memory_buffer.hpp>
#include <asgaard/events/key_event.hpp>
#include <asgaard/drawing/painter.hpp>

#define TOPBAR_HEIGHT_VIOARR 35

class SdlWindowContent final : public Asgaard::SubSurface {
public:
    SdlWindowContent(uint32_t id, const std::shared_ptr<Asgaard::Screen>& screen, const Asgaard::Rectangle& dimensions, void* sdlContext);

    void SetWindowGrab(bool set);

    void CreateWindowBuffer(enum Asgaard::PixelFormat format, enum Asgaard::MemoryBuffer::Flags flags);
    void DeleteWindowBuffer();
    void Redraw();

    const std::shared_ptr<Asgaard::MemoryBuffer>& GetBuffer() const { return m_buffer; }

private:
    void OnKeyEvent(const Asgaard::KeyEvent& keyEvent) override;
    void OnMouseEnter(const std::shared_ptr<Asgaard::Pointer>&, int localX, int localY) override;
    void OnMouseLeave(const std::shared_ptr<Asgaard::Pointer>&) override;
    void OnMouseMove(const std::shared_ptr<Asgaard::Pointer>&, int localX, int localY) override;
    void OnMouseClick(const std::shared_ptr<Asgaard::Pointer>&, enum Asgaard::Pointer::Buttons button, bool pressed) override;

private:
    std::shared_ptr<Asgaard::MemoryPool>   m_memory;
    std::shared_ptr<Asgaard::MemoryBuffer> m_buffer;
    std::shared_ptr<Asgaard::Pointer>      m_currentPointer;

    void* m_sdlContext;
};

class SdlWindow final : public Asgaard::WindowBase {
public:
    SdlWindow(uint32_t id, const std::shared_ptr<Asgaard::Screen>& screen, const Asgaard::Rectangle& dimensions, void* sdlContext);
    ~SdlWindow() { }
    
    void ShowWindow();
    void HideWindow();
    void RaiseWindow();
    void RestoreWindow();
    void SetWindowGrab(bool set);
    bool IsCreated() const { return m_created; }

    void CreateWindowBuffer(enum Asgaard::PixelFormat format, enum Asgaard::MemoryBuffer::Flags flags);
    void DeleteWindowBuffer();
    void RequestRedraw();

    const std::shared_ptr<Asgaard::MemoryBuffer>& GetBuffer() const { return m_content->GetBuffer(); }

private:
    void OnCreated() override;
    void OnRefreshed(const Asgaard::MemoryBuffer* buffer) override;
    void OnKeyEvent(const Asgaard::KeyEvent& keyEvent) override;
    void OnMinimize() override;
    void OnMaximize() override;
    void OnResized(enum SurfaceEdges, int width, int height) override;
    void OnFocus(bool) override;

    void Redraw();
    
private:
    std::shared_ptr<Asgaard::MemoryPool>   m_memory;
    std::shared_ptr<Asgaard::MemoryBuffer> m_buffer;
    std::shared_ptr<SdlWindowContent>      m_content;

    bool  m_created;
    void* m_sdlContext;
    bool  m_redraw;
    std::atomic<bool> m_redrawReady;
};


#include "../../SDL_internal.h"
#include "SDL_vioarrvideo.h"
extern int  SDL_VALI_CreateWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_DestroyWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_SetWindowTitle(_THIS, SDL_Window * window);
extern void SDL_VALI_SetWindowIcon(_THIS, SDL_Window * window, SDL_Surface * icon);
extern void SDL_VALI_SetWindowPosition(_THIS, SDL_Window * window);
extern void SDL_VALI_SetWindowSize(_THIS, SDL_Window * window);
extern int  SDL_VALI_GetWindowBordersSize(_THIS, SDL_Window * window, int *top, int *left, int *bottom, int *right);
extern void SDL_VALI_ShowWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_HideWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_RaiseWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_MaximizeWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_MinimizeWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_RestoreWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_SetWindowBordered(_THIS, SDL_Window * window, SDL_bool bordered);
extern void SDL_VALI_SetWindowResizable(_THIS, SDL_Window * window, SDL_bool resizable);
extern void SDL_VALI_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen);
extern void SDL_VALI_SetWindowMouseGrab(_THIS, SDL_Window * window, SDL_bool grabbed);

#endif // !SDL_core_vioarr_window_h_
