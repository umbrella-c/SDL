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

#if SDL_VIDEO_DRIVER_VALI

#include "SDL_valivideo.h"
#include "SDL_valievents.h"
#include "SDL_valiframebuffer.h"
#include "SDL_loadso.h"
#include "SDL_valiosmesa.h"
#include <asgaard/application.hpp>
#include <GL/osmesa.h>

struct SDL_GLDriverData
{
   OSMesaContext GLAPIENTRY (*CreateContext)        ( GLenum format, OSMesaContext sharelist );
   OSMesaContext GLAPIENTRY (*CreateContextExt)     ( GLenum format, GLint depthBits, GLint stencilBits, GLint accumBits, OSMesaContext sharelist);
   OSMesaContext GLAPIENTRY (*CreateContextAttribs) ( const int *attribList, OSMesaContext sharelist );
   void GLAPIENTRY          (*DestroyContext)       ( OSMesaContext ctx );
   GLboolean GLAPIENTRY     (*MakeCurrent)          ( OSMesaContext ctx, void *buffer, GLenum type, GLsizei width, GLsizei height );
   OSMesaContext GLAPIENTRY (*GetCurrentContext)    ( void );
   OSMESAproc GLAPIENTRY    (*GetProcAddress)       ( const char *funcName );
   void GLAPIENTRY          (*glFinish)             ( void );
};

static const char* g_osmesaDllPaths[1] = {
   "$bin/gallium-osmesa.dll"
};

int VALI_OSGL_LoadLibrary(_THIS, const char *path)
{
   if (!path) {
      for (int i = 0; i < SIZEOF_ARRAY(g_osmesaDllPaths); i++) {
         _this->gl_config.dll_handle = SDL_LoadObject(g_osmesaDllPaths[i]);
         if (!_this->gl_config.dll_handle) {
            return -1;
         }
      }
   }
   else {
      _this->gl_config.dll_handle = SDL_LoadObject(path);
      if (!_this->gl_config.dll_handle) {
         return -1;
      }
   }

    _this->gl_data = (struct SDL_GLDriverData *) SDL_calloc(1, sizeof(struct SDL_GLDriverData));
    if (!_this->gl_data) {
        return SDL_OutOfMemory();
    }

   // load entry points
   _this->gl_data->CreateContext = (OSMesaContext GLAPIENTRY(*)(GLenum, OSMesaContext))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaCreateContext");
   _this->gl_data->CreateContextExt = (OSMesaContext GLAPIENTRY(*)(GLenum, GLint, GLint, GLint, OSMesaContext))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaCreateContextExt");
   _this->gl_data->CreateContextAttribs = (OSMesaContext GLAPIENTRY(*)(const int *, OSMesaContext))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaCreateContextAttribs");
   _this->gl_data->DestroyContext = (void GLAPIENTRY(*)(OSMesaContext))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaDestroyContext");
   _this->gl_data->MakeCurrent = (GLboolean GLAPIENTRY(*)(OSMesaContext, void *, GLenum, GLsizei, GLsizei))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaMakeCurrent");
   _this->gl_data->GetCurrentContext = (OSMesaContext GLAPIENTRY(*)(void))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaGetCurrentContext");
   _this->gl_data->GetProcAddress = (OSMESAproc GLAPIENTRY(*)(const char*))SDL_LoadFunction(_this->gl_config.dll_handle, "OSMesaGetProcAddress");

   if (!_this->gl_data->CreateContext || !_this->gl_data->CreateContextExt ||
       !_this->gl_data->CreateContextAttribs || !_this->gl_data->DestroyContext ||
       !_this->gl_data->MakeCurrent || !_this->gl_data->GetCurrentContext || 
       !_this->gl_data->GetProcAddress)
   {
      return SDL_SetError("Could not retrieve OpenGL functions");
   }

   _this->gl_data->glFinish = (void GLAPIENTRY(*)(void))VALI_OSGL_GetProcAddress(_this, "glFinish");

   return 0;
}


void* VALI_OSGL_GetProcAddress(_THIS, const char *proc)
{
    void *func;
    SDL_SetError("VALI_OSGL_CreateContext(proc=%s)", proc);

    if (!_this->gl_config.dll_handle || !_this->gl_data || !_this->gl_data->GetProcAddress) {
        SDL_SetError("OpenGL function[GetProcAddress]/data was not loaded properly");
        return NULL;
    }

    func = (void*)_this->gl_data->GetProcAddress(proc);
    if (!func) {
        func = (void*)SDL_LoadFunction(_this->gl_config.dll_handle, proc);
    }
    
    return func;
}

void VALI_OSGL_UnloadLibrary(_THIS)
{
    if (!_this->gl_config.dll_handle || !_this->gl_data) {
        SDL_SetError("OpenGL context was not loaded, cannot unload");
        return;
    }

    SDL_UnloadObject(_this->gl_config.dll_handle);
    _this->gl_config.dll_handle = NULL;
    SDL_free(_this->gl_data);
    _this->gl_data = NULL;
}

struct ValiOsMesaContext {
    OSMesaContext context;
    void*         framebuffer;
    int           width;
    int           height;
};

SDL_GLContext VALI_OSGL_CreateContext(_THIS, SDL_Window * window)
{
    struct ValiOsMesaContext* context;
    int attributes[32], n = 0;
    int pitch;
    int result;
    Uint32 format;
    SDL_SetError("VALI_OSGL_CreateContext()");

    if (!_this->gl_config.dll_handle || !_this->gl_data || !_this->gl_data->CreateContextAttribs) {
        SDL_SetError("OpenGL function[CreateContextAttribs]/data was not loaded properly");
        return NULL;
    }

    attributes[n++] = OSMESA_FORMAT;
    attributes[n++] = OSMESA_RGBA; // derive from window framebuffer format
    attributes[n++] = OSMESA_DEPTH_BITS;
    attributes[n++] = 24;
    attributes[n++] = OSMESA_STENCIL_BITS;
    attributes[n++] = 8;
    attributes[n++] = OSMESA_ACCUM_BITS;
    attributes[n++] = 0;
    attributes[n++] = OSMESA_PROFILE;
    attributes[n++] = OSMESA_COMPAT_PROFILE;
    attributes[n++] = OSMESA_CONTEXT_MAJOR_VERSION;
    attributes[n++] = 2;
    attributes[n++] = OSMESA_CONTEXT_MINOR_VERSION;
    attributes[n++] = 1;
    attributes[n++] = 0;

    context = (struct ValiOsMesaContext*) SDL_calloc(1, sizeof(struct ValiOsMesaContext));
    if (!context) {
        return NULL;
    }

    SDL_SetError("VALI_OSGL_CreateContext 1");
    result = _this->CreateWindowFramebuffer(_this, window, &format, &context->framebuffer, &pitch);
    if (result) {
        SDL_free(context);
        return NULL;
    }

    SDL_SetError("VALI_OSGL_CreateContext 2");
    context->context = _this->gl_data->CreateContextAttribs(&attributes[0], NULL);
    if (!context->context) {
        _this->DestroyWindowFramebuffer(_this, window);
        SDL_free(context);
        return NULL;
    }

    SDL_GetWindowSize(window, &context->width, &context->height);
    SDL_SetError("VALI_OSGL_CreateContext 3");

    if (VALI_OSGL_MakeCurrent(_this, window, context)) {
        _this->DestroyWindowFramebuffer(_this, window);
        SDL_free(context);
        return NULL;
    }
    SDL_SetError("VALI_OSGL_CreateContext 4");
    return context;
}

int VALI_OSGL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
    struct ValiOsMesaContext* _context;
    int                       status;
    SDL_SetError("VALI_OSGL_MakeCurrent()");

    if (!_this->gl_config.dll_handle || !_this->gl_data || !_this->gl_data->MakeCurrent) {
        return SDL_SetError("OpenGL function[MakeCurrent]/data was not loaded properly");
    }

    _context = (struct ValiOsMesaContext*)context;
    status = _this->gl_data->MakeCurrent(
        _context->context, _context->framebuffer, GL_UNSIGNED_BYTE,
        _context->width, _context->height);
    if (status == GL_FALSE) {
        return SDL_SetError("MakeCurrent failed to set current context as active");
    }
    return 0;
}

int VALI_OSGL_SetSwapInterval(_THIS, int interval)
{
    if (!_this->gl_config.dll_handle || !_this->gl_data) {
        return SDL_SetError("OpenGL function[SetSwapInterval]/data was not loaded properly");
    }

    return SDL_Unsupported();
}

int VALI_OSGL_GetSwapInterval(_THIS)
{
    if (!_this->gl_config.dll_handle || !_this->gl_data) {
        return SDL_SetError("OpenGL function[GetSwapInterval]/data was not loaded properly");
    }
    return 0;
}

int VALI_OSGL_SwapWindow(_THIS, SDL_Window * window)
{
    SDL_SetError("VALI_OSGL_SwapWindow()");
    if (!_this->gl_config.dll_handle || !_this->gl_data) {
        return SDL_SetError("OpenGL function[SwapWindow]/data was not loaded properly");
    }

    _this->gl_data->glFinish();
    return SDL_VALI_UpdateWindowFramebuffer(_this, window, NULL, 0);
}

void VALI_OSGL_DeleteContext(_THIS, SDL_GLContext context)
{
    struct ValiOsMesaContext* _context;
    SDL_SetError("VALI_OSGL_DeleteContext()");
    if (!_this->gl_config.dll_handle || !_this->gl_data) {
        SDL_SetError("OpenGL function[DeleteContext]/data was not loaded properly");
        return;
    }

    _context = (struct ValiOsMesaContext*)context;
    _this->gl_data->DestroyContext(_context->context);
    //_this->DestroyWindowFramebuffer(_this, window);
    SDL_free(_context);
}

#endif /* SDL_VIDEO_DRIVER_VALI */

/* vi: set ts=4 sw=4 expandtab: */
