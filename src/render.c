#include <math.h>

#include "subassembly.h"

extern NandGate **gates;
extern int nGates;
extern int tickNumber;
extern Subassembly **subassemblies;
extern int nSubassemblies;

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

void logSDLError(const char *msg) {
  fprintf(stderr, "%s error: %s\n", msg, SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
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

void renderGrid(SDL_Renderer *renderer, Vec2 pan, float zoom) {
  int baseSpan = 100;
  int span = baseSpan;

  if (zoom < 1.0f) {
    int scale = (int)log2(1.0f / zoom);
    span = baseSpan << scale;
  } else {
  }

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);

  for (int x = (int)(-pan.x + span / 2) % span; x * zoom < dm.w; x += span) {
    line(renderer, pan, zoom, x, 0, x, dm.h / zoom, 25, 25, 25, 255);
  }

  for (int y = (int)(-pan.y + span / 2) % span; y * zoom < dm.h; y += span) {
    line(renderer, pan, zoom, 0, y, dm.w / zoom, y, 25, 25, 25, 255);
  }

  for (int x = (int)(-pan.x) % span; x * zoom < dm.w; x += span) {
    line(renderer, pan, zoom, x, 0, x, dm.h / zoom, 50, 50, 50, 255);
  }

  for (int y = (int)(-pan.y) % span; y * zoom < dm.h; y += span) {
    line(renderer, pan, zoom, 0, y, dm.w / zoom, y, 50, 50, 50, 255);
  }
}

void render(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan, float zoom,
            const char *statusline, const char *currentSubassemblyName) {

  char tickString[100];
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  renderGrid(renderer, pan, zoom);

  sprintf(tickString, "Tick: %d", tickNumber);
  renderText(renderer, font, 10, 10, tickString);

  for (int i = 0; i < nSubassemblies; i++) {
    drawSubassembly(renderer, font, pan, zoom, subassemblies[i]);
  }

  for (int i = 0; i < nGates; i++) {
    drawNandGate(renderer, pan, zoom, gates[i]);
  }

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  int y = dm.h - 60;
  if (statusline) {
    renderText(renderer, font, 10, y - 25, ":");
    if (strlen(statusline) != 0) {
      renderText(renderer, font, 20, y - 25, statusline);
    }
  }

  if (currentSubassemblyName) {
    renderText(renderer, font, 300, 10, currentSubassemblyName);
  }

  SDL_RenderPresent(renderer);
}
