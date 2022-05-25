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
 * Sdl - Application Window
 */

#include "SDL_vioarr_window.h"
#include <asgaard/drawing/image.hpp>
#include <asgaard/drawing/painter.hpp>
#include <asgaard/object_manager.hpp>
#include <asgaard/pointer.hpp>
#include <vioarr/keycodes.h>
#include "SDL_keycode.h"
#include "SDL_events.h"

extern "C" {
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_mouse_c.h"
#include "../../events/SDL_windowevents_c.h"
}

static SDL_Scancode g_sdlScanCodesMap[VKC_KEYCOUNT] = { 
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_0,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_SEPARATOR,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_PERIOD,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_UNKNOWN, // tick
    SDL_SCANCODE_UNKNOWN, // backtick
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_CLEAR,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_PAUSE,
    SDL_SCANCODE_CAPSLOCK,
    SDL_SCANCODE_UNKNOWN, // hyphen
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_BACKSLASH,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_END,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_SELECT,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_EXECUTE,
    SDL_SCANCODE_PRINTSCREEN,
    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_HELP,
    SDL_SCANCODE_NUMLOCKCLEAR,
    SDL_SCANCODE_SCROLLLOCK,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_LCTRL,
    SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_LGUI,
    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,
    SDL_SCANCODE_F13,
    SDL_SCANCODE_F14,
    SDL_SCANCODE_F15,
    SDL_SCANCODE_F16,
    SDL_SCANCODE_F17,
    SDL_SCANCODE_F18,
    SDL_SCANCODE_F19,
    SDL_SCANCODE_F20,
    SDL_SCANCODE_F21,
    SDL_SCANCODE_F22,
    SDL_SCANCODE_F23,
    SDL_SCANCODE_F24,
    SDL_SCANCODE_AC_FORWARD,
    SDL_SCANCODE_AC_REFRESH,
    SDL_SCANCODE_AC_STOP,
    SDL_SCANCODE_AC_SEARCH,
    SDL_SCANCODE_AC_BOOKMARKS,
    SDL_SCANCODE_AC_HOME,
    SDL_SCANCODE_MUTE,
    SDL_SCANCODE_VOLUMEDOWN,
    SDL_SCANCODE_VOLUMEUP,
    SDL_SCANCODE_AUDIONEXT,
    SDL_SCANCODE_AUDIOPREV,
    SDL_SCANCODE_AUDIOSTOP,
    SDL_SCANCODE_AUDIOPLAY,
    SDL_SCANCODE_MAIL,
    SDL_SCANCODE_MEDIASELECT,
    SDL_SCANCODE_APP1,
    SDL_SCANCODE_APP2,
    SDL_SCANCODE_POWER,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_SLEEP,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_UNKNOWN
};

SdlWindowContent::SdlWindowContent(uint32_t id, const std::shared_ptr<Asgaard::Screen>& screen, const Asgaard::Rectangle& dimensions, void* sdlContext)
    : SubSurface(id, screen, dimensions),
      m_memory(nullptr),
      m_buffer(nullptr),
      m_currentPointer(nullptr),
      m_sdlContext(sdlContext)
{
    MarkInputRegion(Dimensions());
    ApplyChanges();
}

void SdlWindowContent::OnKeyEvent(const Asgaard::KeyEvent& keyEvent)
{
    // Convert to SDL scancode
    SDL_SendKeyboardKey(
        keyEvent.Pressed() ? SDL_PRESSED : SDL_RELEASED, 
        g_sdlScanCodesMap[keyEvent.KeyCode()]);
}

void SdlWindowContent::OnMouseEnter(const std::shared_ptr<Asgaard::Pointer>& pointer, int localX, int localY)
{
    m_currentPointer = pointer;
    SDL_SetMouseFocus(static_cast<SDL_Window*>(m_sdlContext));
}

void SdlWindowContent::OnMouseLeave(const std::shared_ptr<Asgaard::Pointer>& pointer)
{
    if (m_currentPointer->Id() == pointer->Id()) {
        m_currentPointer.reset();
    }
    SDL_SetMouseFocus(nullptr);
}

void SdlWindowContent::OnMouseMove(const std::shared_ptr<Asgaard::Pointer>& pointer, int localX, int localY)
{
    SDL_Mouse* mouse = SDL_GetMouse();
    SDL_SendMouseMotion(static_cast<SDL_Window*>(m_sdlContext), mouse->mouseID, 0, localX, localY);
}

void SdlWindowContent::OnMouseClick(const std::shared_ptr<Asgaard::Pointer>&, enum Asgaard::Pointer::Buttons button, bool pressed)
{
    SDL_Mouse* mouse = SDL_GetMouse();
    int sdlIndex = static_cast<int>(button) + 1;

    if (pressed) {
        SDL_SendMouseButton(static_cast<SDL_Window*>(m_sdlContext), mouse->mouseID, SDL_PRESSED, sdlIndex);
    }
    else {
        SDL_SendMouseButton(static_cast<SDL_Window*>(m_sdlContext), mouse->mouseID, SDL_RELEASED, sdlIndex);
    }
}

void SdlWindowContent::SetWindowGrab(bool set)
{
    // keep track of current mouse
    if (m_currentPointer) {
        if (set) GrabPointer(m_currentPointer);
        else     UngrabPointer(m_currentPointer);
    }
}

void SdlWindowContent::CreateWindowBuffer(enum Asgaard::PixelFormat format, enum Asgaard::MemoryBuffer::Flags flags)
{
    if (!m_memory) {
        auto screenSize = GetScreen()->GetCurrentWidth() * GetScreen()->GetCurrentHeight() * 4;
        m_memory = Asgaard::MemoryPool::Create(this, screenSize);
    }

    m_buffer = Asgaard::MemoryBuffer::Create(this, m_memory, 0, Dimensions().Width(),
        Dimensions().Height(), format, flags);
    SetBuffer(m_buffer);
}

void SdlWindowContent::DeleteWindowBuffer()
{
    if (m_buffer) {
        m_buffer->Unsubscribe(this);
        m_buffer->Destroy();
        m_buffer.reset();
    }
}

void SdlWindowContent::Redraw()
{
    MarkDamaged(Dimensions());
    ApplyChanges();
}

SdlWindow::SdlWindow(uint32_t id, const std::shared_ptr<Asgaard::Screen>& screen, const Asgaard::Rectangle& dimensions, void* sdlContext)
    : WindowBase(id, screen, Asgaard::Rectangle(dimensions.X(), dimensions.Y(), dimensions.Width(), dimensions.Height() + TOPBAR_HEIGHT_VIOARR))
    , m_memory(nullptr)
    , m_buffer(nullptr)
    , m_content(nullptr)
    , m_created(false)
    , m_sdlContext(sdlContext)
    , m_redraw(false)
    , m_redrawReady(false)
{
}

void SdlWindow::OnCreated()
{
    auto screenSize = GetScreen()->GetCurrentWidth() * GetScreen()->GetCurrentHeight() * 4;
    m_memory = Asgaard::MemoryPool::Create(this, screenSize);
    m_buffer = Asgaard::MemoryBuffer::Create(this, m_memory, 0, Dimensions().Width(),
        Dimensions().Height(), Asgaard::PixelFormat::X8B8G8R8);

    // create the content
    m_content = Asgaard::SubSurface::Create<SdlWindowContent>(this, Asgaard::Rectangle(
        0, TOPBAR_HEIGHT_VIOARR, Dimensions().Width(), Dimensions().Height() - TOPBAR_HEIGHT_VIOARR
    ), m_sdlContext);

    // initialize the buffer
    auto setupBuffer = [&] {
        Asgaard::Drawing::Painter painter(m_buffer);
        painter.SetFillColor(0, 0, 0);
        painter.RenderFill();
    };
    setupBuffer();
    SetBuffer(m_buffer);

    // show decoration per default
    EnableDecoration(true);
    SetDropShadow(Asgaard::Rectangle(-10, -10, 20, 30));
    OnRefreshed(nullptr);
    ApplyChanges();
    
    // mark us valid
    m_created = true;
}

void SdlWindow::OnRefreshed(const Asgaard::MemoryBuffer* buffer)
{
    // Request redraw
    if (m_redraw) {
        m_content->Redraw();
        m_redraw = false;
        Redraw();
    }
    else {
        m_redrawReady.store(true);
    }
}

void SdlWindow::OnKeyEvent(const Asgaard::KeyEvent& keyEvent)
{
    // Convert to SDL scancode
    SDL_SendKeyboardKey(
        keyEvent.Pressed() ? SDL_PRESSED : SDL_RELEASED, 
        g_sdlScanCodesMap[keyEvent.KeyCode()]);
}

void SdlWindow::OnMinimize()
{
    SDL_SendWindowEvent(static_cast<SDL_Window*>(m_sdlContext), SDL_WINDOWEVENT_MINIMIZED, 0, 0);
}

void SdlWindow::OnMaximize()
{
    SDL_SendWindowEvent(static_cast<SDL_Window*>(m_sdlContext), SDL_WINDOWEVENT_MAXIMIZED, 0, 0);
}

void SdlWindow::OnResized(enum SurfaceEdges, int width, int height)
{
    SDL_SendWindowEvent(static_cast<SDL_Window*>(m_sdlContext), SDL_WINDOWEVENT_RESIZED, width, height);
}

void SdlWindow::OnFocus(bool focus)
{
    SDL_SendWindowEvent(static_cast<SDL_Window*>(m_sdlContext), 
        (focus ? SDL_WINDOWEVENT_FOCUS_GAINED : SDL_WINDOWEVENT_FOCUS_LOST), 
        0, 0);
}

void SdlWindow::RequestRedraw()
{
    bool shouldRedraw = m_redrawReady.exchange(false);
    if (shouldRedraw) {
        m_content->Redraw();
        Redraw();
    }
    else {
        m_redraw = true;
    }
}

void SdlWindow::Redraw()
{
    MarkDamaged(Dimensions());
    ApplyChanges();
}

void SdlWindow::CreateWindowBuffer(enum Asgaard::PixelFormat format, enum Asgaard::MemoryBuffer::Flags flags)
{
    m_content->CreateWindowBuffer(format, flags);
}

void SdlWindow::DeleteWindowBuffer()
{
    m_content->DeleteWindowBuffer();
}

void SdlWindow::ShowWindow()
{
    if (m_buffer) {
        SetBuffer(m_buffer);
        ApplyChanges();
    }
}

void SdlWindow::HideWindow()
{
    auto nullPointer = std::shared_ptr<Asgaard::MemoryBuffer>(nullptr);
    SetBuffer(nullPointer);
    ApplyChanges();
}

void SdlWindow::RaiseWindow()
{
    // no-op, not implemented
}

void SdlWindow::RestoreWindow()
{
    // what do?
}

void SdlWindow::SetWindowGrab(bool set)
{
    m_content->SetWindowGrab(set);
}

int SDL_VALI_CreateWindow(_THIS, SDL_Window * window)
{
    int w, h, update = 0;

    SDL_GetWindowSize(window, &w, &h);

    auto screen = Asgaard::APP.GetScreen();
    if (screen == nullptr) {
        return -1;
    }

    if (w > screen->GetCurrentWidth())  { w = screen->GetCurrentWidth();  update = 1; }
    if (h > screen->GetCurrentHeight()) { h = screen->GetCurrentHeight(); update = 1; }

    auto sdlWindow = screen->CreateWindow<SdlWindow>(Asgaard::Rectangle(-1, -1, w, h), static_cast<void*>(window));
    if (sdlWindow == nullptr) {
        return -1;
    }

    // wait for window to be created as we run in asynchronous environment
    // and SDL expects the window to be created before it returns
    while (!sdlWindow->IsCreated()) {
        Asgaard::APP.PumpMessages();
    }
    
    SDL_SetWindowData(window, VIOARR_WINDOW_DATA, sdlWindow.get());

    if (update) {
        SDL_SetWindowSize(window, w, h);
    }
    return 0;
}

void SDL_VALI_DestroyWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }
    
    sdlWindow->Destroy();
    SDL_SetWindowData(window, VIOARR_WINDOW_DATA, nullptr);
}

void SDL_VALI_SetWindowTitle(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->SetTitle(window->title);
}

void SDL_VALI_SetWindowIcon(_THIS, SDL_Window * window, SDL_Surface * icon)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    Asgaard::PixelFormat format;
    switch (icon->format->format) {
        case SDL_PIXELFORMAT_RGBA32: format = Asgaard::PixelFormat::R8G8B8A8; break;
        case SDL_PIXELFORMAT_ARGB32: format = Asgaard::PixelFormat::A8R8G8B8; break;
        case SDL_PIXELFORMAT_BGRA32: format = Asgaard::PixelFormat::B8G8R8A8; break;
        case SDL_PIXELFORMAT_ABGR32: format = Asgaard::PixelFormat::A8B8G8R8; break;
        case SDL_PIXELFORMAT_XRGB8888: format = Asgaard::PixelFormat::X8R8G8B8; break;
        case SDL_PIXELFORMAT_XBGR8888: format = Asgaard::PixelFormat::X8B8G8R8; break;
        default: format = Asgaard::PixelFormat::A8R8G8B8; break;
    }

    SDL_LockSurface(icon);
    auto imageptr = std::make_shared<Asgaard::Drawing::Image>(icon->pixels, format, icon->w, icon->h);
    sdlWindow->SetIconImage(imageptr);
    SDL_UnlockSurface(icon);
}

void SDL_VALI_SetWindowPosition(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    // no-op
}

void SDL_VALI_SetWindowSize(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    // no-op
}

int SDL_VALI_GetWindowBordersSize(_THIS, SDL_Window * window, int *top, int *left, int *bottom, int *right)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return -1;
    }

    *top = 35;
    *left = 1;
    *bottom = 1;
    *right = 1;
    return 0;
}

void SDL_VALI_ShowWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->ShowWindow();
}

void SDL_VALI_HideWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->HideWindow();
}

void SDL_VALI_RaiseWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->RaiseWindow();
}

void SDL_VALI_MaximizeWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->RequestFullscreenMode(SdlWindow::FullscreenMode::NORMAL);
}

void SDL_VALI_MinimizeWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->HideWindow();
}

void SDL_VALI_RestoreWindow(_THIS, SDL_Window * window)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->RestoreWindow();
}

void SDL_VALI_SetWindowBordered(_THIS, SDL_Window * window, SDL_bool bordered)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->EnableDecoration(bordered == SDL_TRUE ? true : false);
}

void SDL_VALI_SetWindowResizable(_THIS, SDL_Window * window, SDL_bool resizable)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->SetResizable(resizable == SDL_TRUE ? true : false);
}

void SDL_VALI_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    if (fullscreen == SDL_TRUE) {
        sdlWindow->RequestFullscreenMode(SdlWindow::FullscreenMode::NORMAL);
    }
    else {
        sdlWindow->RequestFullscreenMode(SdlWindow::FullscreenMode::EXIT);
    }
}

void SDL_VALI_SetWindowMouseGrab(_THIS, SDL_Window * window, SDL_bool grabbed)
{
    auto sdlWindow = (SdlWindow*) SDL_GetWindowData(window, VIOARR_WINDOW_DATA);
    if (sdlWindow == nullptr) {
        return;
    }

    sdlWindow->SetWindowGrab(grabbed == SDL_TRUE ? true : false);
}
