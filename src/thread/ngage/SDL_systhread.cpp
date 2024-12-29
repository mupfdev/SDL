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

extern "C" {
#include "../SDL_systhread.h"
#include "../SDL_thread_c.h"
};

static int object_count;

static int RunThread(TAny* data)
{
    SDL_RunThread((SDL_Thread*)data);
    return 0;
}

static TInt NewThread(const TDesC& aName, TAny* aPtr1, TAny* aPtr2)
{
    TInt result = ((RThread*)(aPtr1))->Create(aName,
        RunThread,
        KDefaultStackSize,
        NULL,
        aPtr2);
    return result;
}

int CreateUnique(TInt (*aFunc)(const TDesC& aName, TAny*, TAny*), TAny* aPtr1, TAny* aPtr2)
{
    TBuf<16> name;
    TInt status = KErrNone;
    do
    {
        object_count++;
        name.Format(_L("SDL_%x"), object_count);
        status = aFunc(name, aPtr1, aPtr2);
    }
    while(status == KErrAlreadyExists);
    return status;
}

void SDL_SYS_SetupThread(const char* name)
{
    (void)name;
    return;
}

bool SDL_SYS_CreateThread(SDL_Thread *thread, SDL_FunctionPointer pfnBeginThread, SDL_FunctionPointer pfnEndThread)
{
    RThread rthread;

    TInt status = CreateUnique(NewThread, &rthread, thread);
    if (status != KErrNone)
    {
        delete(((RThread*)(thread->handle)));
        thread->handle = NULL;
        SDL_SetError("Not enough resources to create thread");
        return 1;
    }

    rthread.Resume();
    thread->handle = rthread.Handle();
    return 0;
}

bool SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
    return 0;
}

void SDL_SYS_WaitThread(SDL_Thread* thread)
{
    RThread t;
    t.Open(thread->threadid);
    if(t.ExitReason() == EExitPending)
    {
        TRequestStatus status;
        t.Logon(status);
        User::WaitForRequest(status);
    }
    t.Close();
}

void SDL_SYS_DetachThread(SDL_Thread* thread)
{
    (void)thread;
    return;
}

SDL_ThreadID SDL_GetCurrentThreadID(void)
{
    RThread current;
    TThreadId id = current.Id();
    return id;
}
