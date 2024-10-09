#include <math.h>

#include "subassembly.h"

void line(SDL_Renderer *renderer, Vec2 pan, float zoom, float x1, float y1,
    float x2, float y2, int r, int g, int b, int a) {
  x1 *= zoom;
  x2 *= zoom;
  y1 *= zoom;
  y2 *= zoom;
  thickLineRGBA(renderer, x1, y1, x2, y2, 1, r, g, b, a);
}

void arc(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float radius, float s, float e,
    int r, int g, int b, int a) {
  x *= zoom;
  y *= zoom;
  radius *= zoom;
  arcRGBA(renderer, x, y, radius, s, e, r, g, b, a);
}

void box(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float w,
    float h, int r, int g, int b, int a) {
  x *= zoom;
  y *= zoom;
  w *= zoom;
  h *= zoom;
  boxRGBA(renderer, x, y, w, h, r, g, b, a);
}

void rect(SDL_Renderer *renderer, Vec2 pan, float zoom, float x, float y, float w,
    float h, int r, int g, int b, int a) {
  x *= zoom;
  y *= zoom;
  w *= zoom;
  h *= zoom;
  rectangleRGBA(renderer, x, y, w, h, r, g, b, a);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                const char *text) {
  SDL_Color color = {255, 255, 255, 255};
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
  if (!textSurface) {
    printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return;
  }

  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (!textTexture) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    SDL_FreeSurface(textSurface);
    return;
  }

  SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
  SDL_FreeSurface(textSurface);
  SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
  SDL_DestroyTexture(textTexture);
}
