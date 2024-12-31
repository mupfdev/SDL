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

#include "SDL_ngagevideo.h"

#include <w32std.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../../events/SDL_events_c.h"
#include "../../events/SDL_keyboard_c.h"

#ifdef __cplusplus
}
#endif

void DisableKeyBlocking(SDL_VideoDevice* _this);
int  HandleWsEvent(SDL_VideoDevice *_this, const TWsEvent &aWsEvent);

void NGAGE_PumpEvents(SDL_VideoDevice *_this)
{
    SDL_VideoData *phdata = (SDL_VideoData *)_this->internal;

    while (phdata->NGAGE_WsEventStatus != KRequestPending) {
        phdata->NGAGE_WsSession.GetEvent(phdata->NGAGE_WsEvent);

        HandleWsEvent(_this, phdata->NGAGE_WsEvent);

        phdata->NGAGE_WsEventStatus = KRequestPending;
        phdata->NGAGE_WsSession.EventReady(&phdata->NGAGE_WsEventStatus);
    }
}

static SDL_Scancode ConvertScancode(SDL_VideoDevice *_this, int key)
{
    SDL_Keycode keycode;

    switch (key) {
    case EStdKeyBackspace: // Clear key
        keycode = SDLK_BACKSPACE;
        break;
    case 0x31: // 1
        keycode = SDLK_1;
        break;
    case 0x32: // 2
        keycode = SDLK_2;
        break;
    case 0x33: // 3
        keycode = SDLK_3;
        break;
    case 0x34: // 4
        keycode = SDLK_4;
        break;
    case 0x35: // 5
        keycode = SDLK_5;
        break;
    case 0x36: // 6
        keycode = SDLK_6;
        break;
    case 0x37: // 7
        keycode = SDLK_7;
        break;
    case 0x38: // 8
        keycode = SDLK_8;
        break;
    case 0x39: // 9
        keycode = SDLK_9;
        break;
    case 0x30: // 0
        keycode = SDLK_0;
        break;
    case 0x2a: // Asterisk
        keycode = SDLK_ASTERISK;
        break;
    case EStdKeyHash: // Hash
        keycode = SDLK_HASH;
        break;
    case EStdKeyDevice0: // Left softkey
        keycode = SDLK_SOFTLEFT;
        break;
    case EStdKeyDevice1: // Right softkey
        keycode = SDLK_SOFTRIGHT;
        break;
    case EStdKeyApplication0: // Call softkey
        keycode = SDLK_CALL;
        break;
    case EStdKeyApplication1: // End call softkey
        keycode = SDLK_ENDCALL;
        break;
    case EStdKeyDevice3: // Middle softkey
        keycode = SDLK_SELECT;
        break;
    case EStdKeyUpArrow: // Up arrow
        keycode = SDLK_UP;
        break;
    case EStdKeyDownArrow: // Down arrow
        keycode = SDLK_DOWN;
        break;
    case EStdKeyLeftArrow: // Left arrow
        keycode = SDLK_LEFT;
        break;
    case EStdKeyRightArrow: // Right arrow
        keycode = SDLK_RIGHT;
        break;
    default:
        keycode = SDLK_UNKNOWN;
        break;
    }

    return SDL_GetScancodeFromKey(keycode, (SDL_Keymod*)((void*)0) /* Standard NULL. */);
}

void DisableKeyBlocking(SDL_VideoDevice *_this)
{
    SDL_VideoData *phdata = (SDL_VideoData*)_this->internal;
    TRawEvent      event;

    event.Set((TRawEvent::TType) /*EDisableKeyBlock*/ 51);
    phdata->NGAGE_WsSession.SimulateRawEvent(event);
}

int HandleWsEvent(SDL_VideoDevice *_this, const TWsEvent &aWsEvent)
{
    SDL_VideoData *phdata = (SDL_VideoData *)_this->internal;
    int posted = 0;

    switch (aWsEvent.Type()) {
    case EEventKeyDown: /* Key events */
        SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, ConvertScancode(_this, aWsEvent.Key()->iScanCode), true);
        break;
    case EEventKeyUp: /* Key events */
        SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, ConvertScancode(_this, aWsEvent.Key()->iScanCode), false);
        break;
    case EEventFocusGained: // SDL window got focus.
        phdata->NGAGE_IsWindowFocused = ETrue;
        DisableKeyBlocking(_this);
        phdata->NGAGE_Renderer->StartDirectScreenAccess();
        break;
    case EEventFocusLost: // SDL window lost focus.
    {
        phdata->NGAGE_IsWindowFocused = EFalse;
        phdata->NGAGE_Renderer->StopDirectScreenAccess();
        break;
    }
    case EEventModifiersChanged:
        break;
    default:
        break;
    }
    return posted;
}

#endif // SDL_VIDEO_DRIVER_NGAGE
