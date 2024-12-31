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
# undef NULL
#endif
#include <e32std.h>

struct SDL_Mutex
{
    TInt handle;
};

extern TInt CreateUnique(TInt (*aFunc)(const TDesC &aName, TAny *, TAny *), TAny *, TAny *);

static TInt NewMutex(const TDesC &aName, TAny *aPtr1, TAny *)
{
    return ((RMutex *)aPtr1)->CreateGlobal(aName);
}

SDL_Mutex *SDL_CreateMutex(void)
{
    RMutex rmutex;

    TInt status = CreateUnique(NewMutex, &rmutex, 0);
    if (status != KErrNone) {
        SDL_SetError("Couldn't create mutex.");
        return ((SDL_Mutex*)(void*)0); // Standard NULL.
    }
    SDL_Mutex *mutex = new /*(ELeave)*/ SDL_Mutex;
    mutex->handle = rmutex.Handle();
    return (mutex);
}

void SDL_LockMutex(SDL_Mutex *mutex)
{
    if (((void*)0) /* Standard NULL. */ == mutex) {
        SDL_SetError("Passed a NULL mutex.");
        return;
    }

    RMutex rmutex;
    rmutex.SetHandle(mutex->handle);
    rmutex.Wait();
}

bool SDL_TryLockMutex(SDL_Mutex* mutex)
{
    if (((void*)0) /* Standard NULL. */ == mutex) {
        SDL_SetError("Passed a NULL mutex.");
        return true;
    }

    RMutex rmutex;
    rmutex.SetHandle(mutex->handle);

    if (rmutex.Count() == 0) {
        return true;
    }
    else {
        return false;
    }
}

void SDL_UnlockMutex(SDL_Mutex *mutex)
{
    if (((void*)0) /* Standard NULL. */ == mutex) {
        SDL_SetError("Passed a NULL mutex.");
        return;
    }

    RMutex rmutex;
    rmutex.SetHandle(mutex->handle);
    rmutex.Signal();
}

void SDL_DestroyMutex(SDL_Mutex *mutex)
{
    if (mutex) {
        RMutex rmutex;
        rmutex.SetHandle(mutex->handle);
        rmutex.Signal();
        rmutex.Close();
        delete (mutex);
        mutex = NULL;
    }
}
