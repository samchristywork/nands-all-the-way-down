#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_ttf.h>

#include "types.h"

void line(SDL_Renderer *renderer, Vec2 pan, float zoom, float x1, float y1,
    float x2, float y2, int r, int g, int b, int a);

void arc(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float radius, float s, float e,
    int r, int g, int b, int a);

void box(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float w,
    float h, int r, int g, int b, int a);

void rect(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float w,
    float h, int r, int g, int b, int a);

void renderText(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                const char *text);

void render(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan, float zoom,
            const char *statusline, const char *currentSubassemblyName);

#endif
