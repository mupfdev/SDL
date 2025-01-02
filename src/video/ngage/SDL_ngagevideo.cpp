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

static const TUint KOneFPS = 1000000;
static const TUint K10FPS = 93749;
static const TUint K13FPS = 78124;
static const TUint K16FPS = 62499;
static const TUint K22FPS = 46874;
static const TUint K32FPS = 31249;
static const TUint K64FPS = 15624;
static const TUint KMAXFPS = 1;

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
    SDL_free(device->internal);
    SDL_free(device);
}

static bool NGAGE_VideoInit(SDL_VideoDevice *device)
{
    SDL_VideoData *phdata = (SDL_VideoData*)device->internal;

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

    phdata->NGAGE_Renderer = new CRenderer();
    if (!phdata->NGAGE_Renderer) {
        SDL_OutOfMemory();
        return false;
    }

     // Create fullscreen window.
    phdata->NGAGE_Renderer->ConstructL();

    // Activate window.
    phdata->NGAGE_Renderer->ActivateL();

    // Disable system menu in fullscreen mode.
    phdata->NGAGE_Renderer->MakeVisible(EFalse);

    return true;
}

static void NGAGE_VideoQuit(SDL_VideoDevice *device)
{
    SDL_VideoData *phdata = (SDL_VideoData *)device->internal;

    if (phdata && phdata->NGAGE_Renderer) {
        delete phdata->NGAGE_Renderer;
    }

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
    if (iCoeEnv) {
        iCoeEnv->WsSession().Close();
        delete iCoeEnv;
    }

    if (iTimer) {
        iTimer->Cancel();
        delete iTimer;
    }

    if (iDirectScreen) {
        iDirectScreen->Cancel();
        delete iDirectScreen;
        iDirectScreen = 0;
    }

    delete iRenderer;
    iRenderer = 0;
}

void CRenderer::ConstructL()
{
    iCoeEnv = new (ELeave) CCoeEnv();
    if (!iCoeEnv) {
        return;
    }

    iCoeEnv->CCoeEnv::ConstructL();

    if (!iCoeEnv) {
        User::Leave(KErrNotReady);
    }

    TRAPD(err1, CreateWindowL());
    if (err1 != KErrNone) {
        return;
    }

    TRAPD(err2, SetExtentToWholeScreen());
    if (err2 != KErrNone) {
        return;
    }

    TRAPD(err3, iRenderer = iRenderer->NewL());
    if (err3 != KErrNone) {
        return;
    }

    // Set 12 bit mode.
    Window().SetRequiredDisplayMode(EColor4K);

    // Create Direct screen access but do not activate it:
    // Direct screen access has to be active if container becomes focus.
    TRAPD(err4, iDirectScreen = CDirectScreenAccess::NewL(
        iCoeEnv->WsSession(),
        *(iCoeEnv->ScreenDevice()),
        Window(),
        *this));

    if (err4 != KErrNone) {
        return;
    }

    if (!iDirectScreen) {
        User::Leave(KErrNoMemory);
        return;
    }

    // Create a periodical timer for main loop.
    TRAPD(errc, iTimer = CPeriodic::NewL(CActive::EPriorityLow));

    if (errc != KErrNone) {
        return;
    }
}

void CRenderer::StartPeriodic(void)
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

	if (iTimer && !iTimer->IsActive())
	{
		iTimer->Start(KMAXFPS,KMAXFPS,TCallBack (CRenderer::TimerCallback, this));
	}
}

void CRenderer::StopPeriodic(void)
{
    if (iTimer && iTimer->IsActive()) {
        iTimer->Cancel();
    }

    if (iDirectScreen && iDirectScreen->IsActive()) {
        iDirectScreen->Cancel();
    }

    iScreenGc = NULL;
}

void CRenderer::Render(void* pixels, TInt width, TInt height)
{
    if (iScreenGc && iRenderer) {
        TRAPD(err1, iRenderer->BeginScene());
        if (err1 != KErrNone) {
            return;
        }

        TRAPD(err2, iRenderer->Clear(0xff00ff));
        if (err2 != KErrNone) {
            return;
        }

        TRAPD(err3, iRenderer->ClearStatisticCounters());
        if (err3 != KErrNone) {
            return;
        }

        TRAPD(err4, iRenderer->SetTexture(pixels, width, height));
        if (err4 != KErrNone) {
            return;
        }

        TRAPD(err5, iScreenGc->BitBlt(TPoint(0, 0), iRenderer->Bitmap()));
        if (err5 != KErrNone) {
            return;
        }

        TRAPD(err6, iRenderer->EndScene());
        if (err6 != KErrNone) {
            return;
        }

        TRAPD(err7, iRenderer->Flip(iDirectScreen));
        if (err7 != KErrNone) {
            return;
        }

        // Keep the backlight on.
        User::ResetInactivityTime();
        // Suspend the current thread for a short while.
        // Give some time to other threads and active objects.
        User::After(0);
    }
}

void CRenderer::Restart  (RDirectScreenAccess::TTerminationReasons /*aReason*/)
{
    StartPeriodic();
}

void CRenderer::AbortNow (RDirectScreenAccess::TTerminationReasons /*aReason*/)
{
    StopPeriodic();
}

TInt CRenderer::TimerCallback(TAny* aPtr)
{
    RDebug::Print(_L("TimerCallback"));
    (STATIC_CAST(CRenderer*, aPtr))->Render(NULL, 176, 208);
    return ETrue;
}

#endif // SDL_VIDEO_DRIVER_NGAGE
