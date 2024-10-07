#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

char *getString(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan, float zoom,
                const char *prompt, const char *currentSubassemblyName);

#endif
