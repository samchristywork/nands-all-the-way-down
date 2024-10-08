#ifndef NAND_H
#define NAND_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "types.h"

void tick();

int portFromPos(NandGate *g, Vec2 pos);

void drawNandGate(SDL_Renderer *renderer, Vec2 pan, float zoom, NandGate *gate);

NandGate *createNand(Vec2 pos, int n);

void setNumInputs(NandGate *g, int n);

bool checkGateCollision(NandGate *g, Vec2 pos);

void printNandGate(NandGate *ng);

#endif
