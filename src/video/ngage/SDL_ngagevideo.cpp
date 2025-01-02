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
#ifdef __cplusplus
extern "C" {
#endif

#include "../SDL_sysvideo.h"

#ifdef __cplusplus
}
#endif

#ifdef SDL_VIDEO_DRIVER_NGAGE

#include "SDL_ngagevideo.h"
#include "SDL_ngageevents_c.h"
#include "SDL_ngageframebuffer_c.h"

#define NGAGE_VIDEO_DRIVER_NAME "N-Gage"

static void NGAGE_DeleteDevice(SDL_VideoDevice *device);
static bool NGAGE_VideoInit(SDL_VideoDevice *device);
static void NGAGE_VideoQuit(SDL_VideoDevice *device);

static bool NGAGE_GetDisplayBounds(SDL_VideoDevice* _this, SDL_VideoDisplay* display, SDL_Rect* rect);
static bool NGAGE_GetDisplayModes(SDL_VideoDevice* _this, SDL_VideoDisplay* display);

static SDL_VideoDevice* NGAGE_CreateDevice(void)
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

    device->name = "Nokia N-Gage";

    device->VideoInit = NGAGE_VideoInit;
    device->VideoQuit = NGAGE_VideoQuit;

    device->GetDisplayBounds = NGAGE_GetDisplayBounds;
    device->GetDisplayModes = NGAGE_GetDisplayModes;

    device->CreateWindowFramebuffer = NGAGE_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = NGAGE_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = NGAGE_DestroyWindowFramebuffer;

    device->PumpEvents = NGAGE_PumpEvents;

    //device->StartTextInput
    //device->StopTextInput
    //device->UpdateTextInputArea
    //device->ClearComposition

    //device->HasScreenKeyboardSupport
    //device->ShowScreenKeyboard
    //device->HideScreenKeyboard
    //device->SetTextInputProperties
    //device->IsScreenKeyboardShown

    device->free = NGAGE_DeleteDevice;

    device->device_caps = VIDEO_DEVICE_CAPS_FULLSCREEN_ONLY;

    return device;
}

VideoBootStrap NGAGE_bootstrap = { NGAGE_VIDEO_DRIVER_NAME, "N-Gage Video Driver", NGAGE_CreateDevice, (bool (*)(const SDL_MessageBoxData *, int *))((void*)0) /* Standard NULL. */ };

static void NGAGE_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_VideoData *phdata = (SDL_VideoData*)device->internal;

    if (phdata && phdata->NGAGE_Renderer) {
        delete phdata->NGAGE_Renderer;
    }

    SDL_free(device->internal);
    SDL_free(device);
}

static bool NGAGE_VideoInit(SDL_VideoDevice *device)
{
    SDL_VideoData *phdata = (SDL_VideoData*)device->internal;

    TFileName aPath = _L("demo");
    TParse aParser;
    aParser.Set(aPath, NULL, NULL);

    TPtrC aPathLessName = aParser.DriveAndPath();

    SDL_zero(phdata->mode);
    SDL_zero(phdata->display);

    phdata->mode.w = 176;
    phdata->mode.h = 208;
    phdata->mode.refresh_rate = 60.0f;
    phdata->mode.format = SDL_PIXELFORMAT_XRGB4444;

    phdata->display.name = "N-Gage";
    phdata->display.desktop_mode = phdata->mode;

    if (SDL_AddVideoDisplay(&phdata->display, false) == 0) {
        return false;
    }

    // Create renderer.
    phdata->NGAGE_Renderer = new (ELeave) CRenderer;

    // Create fullscreen window.
    CleanupStack::PushL(phdata->NGAGE_Renderer);
    phdata->NGAGE_Renderer->ConstructL(aPathLessName);
    CleanupStack::Pop();

	// Disable system menu if fullscreen only.
	phdata->NGAGE_Renderer->MakeVisible(EFalse);

    // Activate window.
    phdata->NGAGE_Renderer->ActivateL();

    return true;
}

static void NGAGE_VideoQuit(SDL_VideoDevice *device)
{
    return;
}

static bool NGAGE_GetDisplayBounds(SDL_VideoDevice* device, SDL_VideoDisplay* display, SDL_Rect* rect)
{
    if (!display) {
        return false;
    }

    rect->x = 0;
    rect->y = 0;
    rect->w = display->current_mode->w;
    rect->h = display->current_mode->h;

    return true;
}

static bool NGAGE_GetDisplayModes(SDL_VideoDevice* device, SDL_VideoDisplay* display)
{
    SDL_VideoData *phdata = (SDL_VideoData*)device->internal;
    SDL_DisplayMode mode;

    SDL_zero(mode);
    mode.w = phdata->mode.w;
    mode.h = phdata->mode.h;
    mode.refresh_rate = phdata->mode.refresh_rate;
    mode.format = phdata->mode.format;

    if (!SDL_AddFullscreenDisplayMode(display, &mode)) {
        return false;
    }

    return true;
}

CRenderer::~CRenderer()
{
    if (iDirectScreen)
    {
        iDirectScreen->Cancel();
        delete iDirectScreen;
    }

    delete iRenderer;
}

void CRenderer::ConstructL(const TDesC& aPath)
{
    CreateWindowL();
    SetExtentToWholeScreen();

    CleanupStack::PushL(iRenderer);
    iRenderer = CNRenderer::NewL();
    CleanupStack::Pop();

    // Set 12 bit mode.
    Window().SetRequiredDisplayMode(EColor4K);

    // Create Direct screen access but do not activate it:
    // Direct screen access has to be active if container becomes focus.
    CleanupStack::PushL(iDirectScreen);
    iDirectScreen = CDirectScreenAccess::NewL(
        iCoeEnv->WsSession(),
        *(iCoeEnv->ScreenDevice()),
        Window(),
        *this);
    CleanupStack::Pop();
}

void CRenderer::StartDirectScreenAccess(void)
{
    if (iDirectScreen && !iDirectScreen->IsActive()) {
		// Start it.
		TRAPD (err, iDirectScreen->StartL());
		if (KErrNone != err) {
            return;
        }

        // Set auto update.
		iDirectScreen->ScreenDevice()->SetAutoUpdate(ETrue);

        // Save graphic context.
		iScreenGc = iDirectScreen->Gc();

        // Set cliping area for context.
        // If you need screen rect use DirectScreen->DrawingRegion->BoundingRect();
		iScreenGc->SetClippingRegion(iDirectScreen->DrawingRegion());
	}
}

void CRenderer::StopDirectScreenAccess(void)
{
    if (iDirectScreen && iDirectScreen->IsActive()) {
        iDirectScreen->Cancel();
    }

    iScreenGc = NULL;
}

void CRenderer::Restart  (RDirectScreenAccess::TTerminationReasons /*aReason*/)
{
    StartDirectScreenAccess();
}

void CRenderer::AbortNow (RDirectScreenAccess::TTerminationReasons /*aReason*/)
{
	StopDirectScreenAccess();
}

void CRenderer::Render(void* pixels, TInt width, TInt height)
{
    if (iScreenGc && iRenderer) {
        iRenderer->BeginScene();
        iRenderer->Clear(0x000000);
        iRenderer->ClearStatisticCounters();
        iRenderer->SetTexture(pixels, width, height);
        iScreenGc->BitBlt(TPoint(0, 0), iRenderer->Bitmap());
        iRenderer->EndScene();
        iRenderer->Flip(iDirectScreen);
    }
}

#endif // SDL_VIDEO_DRIVER_NGAGE
