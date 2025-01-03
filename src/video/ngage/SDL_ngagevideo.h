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

#include "../SDL_sysvideo.h"

#ifndef _SDL_ngagevideo_h
#define _SDL_ngagevideo_h

#ifdef NULL
#undef NULL
#endif

#include <NRenderer.h>
#include <coecntrl.h>
#include <w32std.h>

class CRenderer: public CCoeControl, public MDirectScreenAccess
{
public:
    void ConstructL(void);

    ~CRenderer();

    // These functions are used when the applications gets or loses focus.
    void StartPeriodic(void);
    void StopPeriodic(void);

    void Render(void* pixels, TInt width, TInt height);

    void Restart  (RDirectScreenAccess::TTerminationReasons aReason);
    void AbortNow (RDirectScreenAccess::TTerminationReasons aReason);

    static TInt TimerCallback(TAny* aPtr);

private:
    // Direct screen access.
    CDirectScreenAccess* iDirectScreen;
    CFbsBitGc *iScreenGc;
    RWindow *iWindow;

    // BackBuffer.
    CNRenderer *iRenderer;

	// Periodic Timer.
	CPeriodic *iTimer;
};

typedef struct SDL_VideoData
{
    RWsSession NGAGE_WsSession;
    TRequestStatus NGAGE_WsEventStatus;
    TWsEvent NGAGE_WsEvent;

    CRenderer* NGAGE_Renderer;

    TBool NGAGE_IsWindowFocused;

    SDL_DisplayMode mode;
    SDL_VideoDisplay display;
    SDL_Window window;

} SDL_VideoData;

#endif // _SDL_ngagevideo_h

#endif // SDL_VIDEO_DRIVER_NGAGE
