#ifndef SUBASSEMBLY_H
#define SUBASSEMBLY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "nand.h"
#include "types.h"

void drawSubassembly(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan,
                     float zoom, Subassembly *subassembly);

Subassembly *createSubassembly(char *name, Rect rect, int nInputs, int nOutputs,
                               char **inputNames, char **outputNames);

void addInput(Subassembly *s, char *name);

void addOutput(Subassembly *s, char *name);

Vec2 getInputsScreenPos(Subassembly *s, int n, Vec2 pan, float zoom);

Vec2 getOutputsScreenPos(Subassembly *s, int n, Vec2 pan, float zoom);

#endif
