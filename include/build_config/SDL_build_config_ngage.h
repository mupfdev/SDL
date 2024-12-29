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

#ifndef SDL_build_config_ngage_h_
#define SDL_build_config_ngage_h_
#define SDL_build_config_h_

#include <SDL3/SDL_platform_defines.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_FSOPS_POSIX        1
#define SDL_HAPTIC_DISABLED    1
#define SDL_MAIN_NEEDED        1
#define SDL_MAIN_USE_CALLBACKS 1
#define SDL_THREAD_NGAGE       1

#define HAVE_ATAN     1
#define HAVE_ATAN2    1
#define HAVE_COPYSIGN 1
#define HAVE_COS      1
#define HAVE_EXP      1
#define HAVE_FABS     1
#define HAVE_FLOOR    1
#define HAVE_FMOD     1
#define HAVE_ISINF    1
#define HAVE_ISINFF   0
#define HAVE_ISNAN    1
#define HAVE_ISNANF   0
#define HAVE_LOG      1
#define HAVE_LOG10    1
#define HAVE_MALLOC   1
#define HAVE_MATH_H   1
#define HAVE_MODF     1
#define HAVE_POW      1
#define HAVE_SCALBN   1
#define HAVE_SIN      1
#define HAVE_STDIO_H  1
#define HAVE_SQRT     1
#define HAVE_TAN      1

#define isnanf(x) SDL_uclibc_isnanf(x)
#define isinff(x) SDL_uclibc_isinff(x);

#if defined(SDL_PLATFORM_NGAGE)
extern void va_copy(char* dest, char* src);
#endif

#endif /* SDL_build_config_ngage_h_ */
