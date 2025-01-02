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
#include <e32def.h>
#include <e32svr.h>
#include <e32base.h>
#include <estlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <w32std.h>
#include <apgtask.h>

TInt E32Main()
{
    /*  Get the clean-up stack */
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) {
        return KErrNoMemory;
    }

    /* Arrange for multi-threaded operation */
    TInt err = SpawnPosixServerThread();
    if (err != KErrNone) {
        delete cleanup;
        return err;
    }

    /* Get args and environment */
    int    argc = 0;
    char** argv = 0;
    char** envp = 0;

    __crt0(argc, argv, envp);

    /* Start the application! */

    /* Create stdlib */
    _REENT;

    /* Set process and thread priority and name */
    RThread  currentThread;
    RProcess thisProcess;
    TParse   exeName;
    exeName.Set(thisProcess.FileName(), NULL, NULL);
    currentThread.Rename(exeName.Name());
    currentThread.SetProcessPriority(EPriorityLow);
    currentThread.SetPriority(EPriorityMuchLess);

    /* Increase heap size */
    RHeap* newHeap  = NULL;
    RHeap* oldHeap  = NULL;
    TInt   heapSize = 7500000;
    int    ret;

    newHeap = User::ChunkHeap(NULL, heapSize, heapSize, KMinHeapGrowBy);

    if (NULL == newHeap)
    {
        ret = 3;
        goto cleanup;
    }
    else
    {
        oldHeap = User::SwitchHeap(newHeap);
        /* Call stdlib main */
        SDL_SetMainReady();
        ret = SDL_main(argc, argv);
    }

cleanup:
    _cleanup();

    if (newHeap) {
        User::SwitchHeap(oldHeap);
        newHeap->Close();
    }

    CloseSTDLIB();
    delete cleanup;
    return ret;
}

#ifdef __cplusplus
extern "C" {
#endif

int SDL_RunApp(int argc, char* argv[], SDL_main_func mainFunction, void* reserved)
{
    return 0;
}

#ifdef __cplusplus
}
#endif
