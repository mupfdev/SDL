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

char *SDL_SYS_GetBasePath(void)
{
    char *base_path = SDL_strdup("E:\\");
    return base_path;
}

char *SDL_SYS_GetPrefPath(const char *org, const char *app)
{
    char pref_path[256];
    SDL_snprintf(pref_path, sizeof(pref_path), "C:\\System\\Apps\\%s\\%s\\", org, app);
    return SDL_strdup(pref_path);
}

char *SDL_SYS_GetUserFolder(SDL_Folder folder)
{
    const char *folder_path = NULL;
    switch (folder) {
    case SDL_FOLDER_HOME:
        folder_path = "C:\\Data\\";
        break;
    case SDL_FOLDER_DESKTOP:
        folder_path = "C:\\Data\\Desktop\\";
        break;
    case SDL_FOLDER_DOCUMENTS:
        folder_path = "C:\\Data\\Documents\\";
        break;
    case SDL_FOLDER_DOWNLOADS:
        folder_path = "C:\\Data\\Downloads\\";
        break;
    case SDL_FOLDER_MUSIC:
        folder_path = "C:\\Data\\Music\\";
        break;
    case SDL_FOLDER_PICTURES:
        folder_path = "C:\\Data\\Pictures\\";
        break;
    case SDL_FOLDER_PUBLICSHARE:
        folder_path = "C:\\Data\\Public\\";
        break;
    case SDL_FOLDER_SAVEDGAMES:
        folder_path = "C:\\Data\\SavedGames\\";
        break;
    case SDL_FOLDER_SCREENSHOTS:
        folder_path = "C:\\Data\\Screenshots\\";
        break;
    case SDL_FOLDER_TEMPLATES:
        folder_path = "C:\\Data\\Templates\\";
        break;
    case SDL_FOLDER_VIDEOS:
        folder_path = "C:\\Data\\Videos\\";
        break;
    default:
        folder_path = "C:\\Data\\";
        break;
    }
    return SDL_strdup(folder_path);
}
