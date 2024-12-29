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

#ifdef NULL
#undef NULL
#endif

#include <e32def.h>
#include <NRenderer.h>
#include "../SDL_sysvideo.h"
#include "SDL_ngageevents_c.h"
#include "SDL_ngageframebuffer_c.h"
#include "SDL_ngagevideo.h"

#define NGAGE_VIDEO_DRIVER_NAME "N-Gage"

static void NGAGE_DeleteDevice(SDL_VideoDevice *device);
static bool NGAGE_VideoInit(SDL_VideoDevice *_this);
static void NGAGE_VideoQuit(SDL_VideoDevice *_this);
static bool NGAGE_GetDisplayModes(SDL_VideoDevice *_this, SDL_VideoDisplay *display);
static bool NGAGE_SetDisplayMode(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_DisplayMode *mode);
static bool NGAGE_GetDisplayBounds(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_Rect *rect);
static bool NGAGE_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props);
static void NGAGE_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window);

static SDL_VideoDevice *NGAGE_CreateDevice(void)
{
    SDL_VideoDevice *device;
    SDL_VideoData *phdata;

    // Initialize all variables that we clean on shutdown.
    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return 0;
    }

    // Initialize internal N-Gage specific data.
    phdata = (SDL_VideoData *)SDL_calloc(1, sizeof(SDL_VideoData));
    if (!phdata) {
        SDL_OutOfMemory();
        SDL_free(device);
        return 0;
    }

    device->internal = phdata;

    device->VideoInit = NGAGE_VideoInit;
    device->VideoQuit = NGAGE_VideoQuit;

    device->GetDisplayModes = NGAGE_GetDisplayModes;
    device->SetDisplayMode = NGAGE_SetDisplayMode;
    device->GetDisplayBounds = NGAGE_GetDisplayBounds;

    device->CreateSDLWindow = NGAGE_CreateWindow;
    device->DestroyWindow = NGAGE_DestroyWindow;

    device->PumpEvents = NGAGE_PumpEvents;

    device->CreateWindowFramebuffer = SDL_NGAGE_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = SDL_NGAGE_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = SDL_NGAGE_DestroyWindowFramebuffer;

    device->free = NGAGE_DeleteDevice;

    device->device_caps = VIDEO_DEVICE_CAPS_FULLSCREEN_ONLY;

    return device;
}

VideoBootStrap NGAGE_bootstrap = { NGAGE_VIDEO_DRIVER_NAME, "N-Gage Video Driver", NGAGE_CreateDevice, (bool (*)(const SDL_MessageBoxData *, int *))NULL_ };

static void NGAGE_DeleteDevice(SDL_VideoDevice *device)
{
    return;
}

static bool NGAGE_VideoInit(SDL_VideoDevice *_this)
{
    SDL_VideoData* phdata = (SDL_VideoData*)_this->internal;

    phdata->NGAGE_Renderer = CNRenderer::NewL();
    if (!phdata->NGAGE_Renderer) {
        delete phdata->NGAGE_Renderer;
        phdata->NGAGE_Renderer = 0;
        return false;
    }

    return true;
}


static void NGAGE_VideoQuit(SDL_VideoDevice *_this)
{
    SDL_VideoData* phdata = (SDL_VideoData*)_this->internal;

    if (phdata->NGAGE_Renderer) {
        delete phdata->NGAGE_Renderer;
        phdata->NGAGE_Renderer = 0;
    }

    return;
}

static bool NGAGE_GetDisplayModes(SDL_VideoDevice *_this, SDL_VideoDisplay *display)
{
    return false;
}

static bool NGAGE_SetDisplayMode(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_DisplayMode *mode)
{
    return false;
}

static bool NGAGE_GetDisplayBounds(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_Rect *rect)
{
    return false;
}

static bool NGAGE_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props)
{
    return true;
}

static void NGAGE_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    return;
}
