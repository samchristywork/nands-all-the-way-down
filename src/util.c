#include <SDL2/SDL_ttf.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "types.h"

char *getString(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan, float zoom,
                const char *prompt, const char *currentSubassemblyName) {
  render(renderer, font, pan, zoom, prompt, currentSubassemblyName);

  size_t bufferSize = 128;
  char *ret = (char *)malloc(bufferSize);
  if (!ret) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  ret[0] = '\0';

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_RETURN:
          quit = true;
          break;

        case SDLK_BACKSPACE:
          if (strlen(ret) > 0) {
            ret[strlen(ret) - 1] = '\0';
          }
          break;

        default:
          if (e.key.keysym.sym < 128) {
            size_t len = strlen(ret);
            if (len + 1 >= bufferSize) {
              bufferSize *= 2;
              ret = (char *)realloc(ret, bufferSize);
              if (!ret) {
                fprintf(stderr, "Memory reallocation failed\n");
                return NULL;
              }
            }

            if ((e.key.keysym.mod & KMOD_SHIFT) ||
                (e.key.keysym.mod & KMOD_CAPS)) {
              ret[len] = (char)toupper(e.key.keysym.sym);
            } else {
              ret[len] = (char)e.key.keysym.sym;
            }
            ret[len + 1] = '\0';
          }
          break;
        }

        render(renderer, font, pan, zoom, ret, currentSubassemblyName);
      }
    }
  }

  return ret;
}
