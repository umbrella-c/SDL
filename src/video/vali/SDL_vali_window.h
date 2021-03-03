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

#ifndef SDL_core_vali_window_h_
#define SDL_core_vali_window_h_
/* ValiOS
 *
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
#include <asgaard/memory_pool.hpp>
#include <asgaard/memory_buffer.hpp>
#include <asgaard/key_event.hpp>
#include <asgaard/drawing/painter.hpp>
#include <os/keycodes.h>

class SdlWindow final : public Asgaard::WindowBase {
public:
    SdlWindow(uint32_t id, const Asgaard::Rectangle& dimensions)
        : WindowBase(id, dimensions)
        , m_memory(nullptr)
        , m_buffer(nullptr)
        , m_decoration(nullptr)
        , m_redraw(false)
        , m_redrawReady(false) { }
    
    ~SdlWindow() { }
    
    void CreateWindowBuffer(enum PixelFormat format);
    void DeleteWindowBuffer();
    void UpdateTitle(const char* title);
    void RequestRedraw();

public:
    const std::shared_ptr<Asgaard::MemoryBuffer>& GetBuffer() const { return m_buffer; }

private:
    void OnCreated(Asgaard::Object* createdObject) override;
    void OnRefreshed(Asgaard::MemoryBuffer* buffer) override;
    void OnKeyEvent(const Asgaard::KeyEvent& keyEvent) override;
    void DescriptorEvent(int iod, unsigned int events) override;
    void Teardown() override;

private:
    void ResetBuffer();
    void Redraw();
    
private:
    std::shared_ptr<Asgaard::MemoryPool>       m_memory;
    std::shared_ptr<Asgaard::MemoryBuffer>     m_buffer;
    std::shared_ptr<Asgaard::WindowDecoration> m_decoration;

    bool m_redraw;
    std::atomic<bool> m_redrawReady;
};


#include "../../SDL_internal.h"
#include "SDL_valivideo.h"
extern int  SDL_VALI_CreateWindow(_THIS, SDL_Window * window);
extern void SDL_VALI_DestroyWindow(_THIS, SDL_Window * window);

#endif // !SDL_core_vali_h_
