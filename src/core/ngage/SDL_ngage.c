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
#include <math.h>
#include <stdarg.h>

unsigned int __clzsi2(unsigned int x)
{
    if (x == 0)
    {
        return 32;
    }
    unsigned int n = 0;

    if (x <= 0x0000FFFF)
    {
        n += 16;
        x <<= 16;
    }
    if (x <= 0x00FFFFFF)
    {
        n += 8;
        x <<= 8;
    }
    if (x <= 0x0FFFFFFF)
    {
        n += 4;
        x <<= 4;
    }
    if (x <= 0x3FFFFFFF)
    {
        n += 2;
        x <<= 2;
    }
    if (x <= 0x7FFFFFFF)
    {
        n += 1;
    }
    return n;
}

double __floatundidf(uint64_t a)
{
    return (double)a;
}

float __floatunsisf(uint32_t a)
{
    return (float)a;
}

bool __unorddf2(double a, double b)
{
    return isnan(a) || isnan(b);
}

void va_copy(char *dest, char *src)
{
    *dest = *src;
}
