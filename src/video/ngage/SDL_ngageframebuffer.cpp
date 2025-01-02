/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

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
#include "SDL_internal.h"

#ifdef SDL_VIDEO_DRIVER_NGAGE

#include "SDL_ngageframebuffer_c.h"
#include "SDL_ngagevideo.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../../SDL_properties_c.h"

#ifdef __cplusplus
}
#endif

#define NGAGE_SURFACE "SDL.internal.window.surface"

bool NGAGE_CreateWindowFramebuffer(SDL_VideoDevice *device, SDL_Window *window, SDL_PixelFormat *format, void **pixels, int *pitch)
{
    SDL_Surface *framebuffer;
    const SDL_DisplayMode *mode;
    int w, h;

    NGAGE_DestroyWindowFramebuffer(device, window);

    mode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
    SDL_GetWindowSizeInPixels(window, &w, &h);
    framebuffer = SDL_CreateSurface(w, h, mode->format);

    if (!framebuffer) {
        return false;
    }

    SDL_SetSurfaceProperty(SDL_GetWindowProperties(window), NGAGE_SURFACE, framebuffer);
    *format = mode->format;
    *pixels = framebuffer->pixels;
    *pitch = framebuffer->pitch;

    return true;
}

bool NGAGE_UpdateWindowFramebuffer(SDL_VideoDevice *device, SDL_Window *window, const SDL_Rect *rects, int numrects)
{
    SDL_VideoData *phdata = (SDL_VideoData *)device->internal;

    RDebug::Print(_L("SDL: NGAGE_UpdateWindowFramebuffer"));

    if (!phdata->NGAGE_Renderer) {
        return false;
    }

    SDL_Surface *surface;

    surface = (SDL_Surface *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), NGAGE_SURFACE, NULL);
    if (!surface) {
        return SDL_SetError("%s: Unable to get the window surface.", __FUNCTION__);
    }

    // tbd.

    return true;
}

void NGAGE_DestroyWindowFramebuffer(SDL_VideoDevice *device, SDL_Window *window)
{
    SDL_ClearProperty(SDL_GetWindowProperties(window), NGAGE_SURFACE);
}

#endif // SDL_VIDEO_DRIVER_NGAGE
