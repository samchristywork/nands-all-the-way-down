#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>

#include "nand.h"
#include "render.h"

extern NandGate **gates;
extern int nGates;
extern int tickNumber;

void tick() {
  tickNumber++;
  for (int i = 0; i < nGates; i++) {
    NandGate *gate = gates[i];

    if (gate->nInputs == 0) {
      gate->nextValue = true;
      continue;
    }

    gate->nextValue = false;
    for (int i = 0; i < gate->nInputs; i++) {
      int inputIdx = gate->inputs[i];
      if (inputIdx != -1) {
        if (!gates[inputIdx]->value) {
          gate->nextValue = true;
          break;
        }
      } else {
        gate->nextValue = true;
        break;
      }
    }
  }

  for (int i = 0; i < nGates; i++) {
    NandGate *gate = gates[i];
    gate->value = gate->nextValue;
  }
}

int portFromPos(NandGate *g, Vec2 pos) {
  if (g->nInputs == 0 || g == NULL) {
    return -1;
  } else if (g->nInputs == 1) {
    Vec2 d = {
      g->pos.x - pos.x,
      g->pos.y - pos.y
    };
    if (d.x * d.x + d.y * d.y < 25 * 25) {
      return 0;
    }
  } else {
    for (int i = 0; i < g->nInputs; i++) {
      float pitch = 30 / (g->nInputs - 1);

      float w = 10;
      float h = 10;
      if (pos.x > g->pos.x - w && pos.x < g->pos.x + w) {
        if (pos.y > g->pos.y - 15 + pitch * i - h &&
            pos.y < g->pos.y - 15 + pitch * i + h) {
          return i;
        }
      }
    }
  }

  return -1;
}

void drawNandGate(SDL_Renderer *renderer, Vec2 pan, float zoom, NandGate *gate) {
  int x = gate->pos.x - pan.x;
  int y = gate->pos.y - pan.y;

  float s = gate->shade;

  line(renderer, pan, zoom, x, y - 20, x, y + 20, s, s, s, 255);
  line(renderer, pan, zoom, x, y - 20, x + 20, y - 20, s, s, s, 255);
  line(renderer, pan, zoom, x, y + 20, x + 20, y + 20, s, s, s, 255);
  arc(renderer, pan, zoom, x + 20, y, 20, 270, 90, s, s, s, 255);
  arc(renderer, pan, zoom, x + 45, y, 5, 0, 359, s, s, s, 255);

  int r = 100;
  int g = 100;
  int b = 255;
  if (gate->value) {
    r = 255;
    g = 255;
    b = 0;
  }
  line(renderer, pan, zoom, x + 50, y, x + 55, y, r, g, b, 255);

  for (int i = 0; i < gate->nInputs; i++) {
    float h = 30;
    float pitch = h / (gate->nInputs - 1);
    float ix = gate->pos.x - 10 - pan.x;
    float iy = gate->pos.y - 15 + i * pitch - pan.y;
    int r = 100;
    int g = 100;
    int b = 255;

    if (gate->nInputs == 1) {
      iy = gate->pos.y - pan.y;
    }

    int gIdx = gate->inputs[i];
    if (gIdx >= 0) {
      NandGate *input = gates[gIdx];
      if (input->value) {
        r = 255;
        g = 255;
        b = 0;
      }

      line(renderer, pan, zoom, input->pos.x - pan.x + 55,
                    input->pos.y - pan.y, ix, iy, r, g, b, 255);
    }
    line(renderer, pan, zoom, ix, iy, ix + 10, iy, r, g, b, 255);

    if (gate->portHighlight == i) {
      arc(renderer, pan, zoom, ix, iy, 5, 0, 359, s, s, s, 255);
    }
  }
}

NandGate *createNand(Vec2 pos, int n) {
  NandGate *gate = malloc(sizeof(NandGate));

  gate->pos.x = pos.x;
  gate->pos.y = pos.y;
  gate->nInputs = n;
  gate->inputs = malloc(sizeof(int) * 2);
  for (int i = 0; i < n; i++) {
    gate->inputs[i] = -1;
  }
  gate->value = false;
  gate->shade = 255;
  gate->portHighlight = -1;

  if (gates == 0) {
    gates = malloc(sizeof(NandGate *));
  } else {
    gates = realloc(gates, sizeof(NandGate *) * (nGates + 1));
  }

  gates[nGates] = gate;

  nGates++;

  return gate;
}

void setNumInputs(NandGate *g, int n) {
  if (!g) {
    return;
  }

  if (n <= g->nInputs) {
    g->nInputs = n;
  } else {
    int oldN = g->nInputs;
    g->nInputs = n;
    g->inputs = realloc(g->inputs, sizeof(int) * n);
    for (int i = oldN; i < n; i++) {
      g->inputs[i] = -1;
    }
  }
}

bool checkGateCollision(NandGate *g, Vec2 pos) {
  int gx = g->pos.x;
  int gy = g->pos.y;

  if (pos.x > gx - 15 && pos.x < gx + 55) {
    if (pos.y > gy - 25 && pos.y < gy + 25) {
      return true;
    }
  }

  return false;
}

void printNandGate(NandGate *ng) {
  if (!ng) {
    printf("NandGate is NULL\n");
    return;
  }

  printf("NandGate Position: x=%f, y=%f\n", ng->pos.x, ng->pos.y);

  printf("Number of Inputs: %d\n", ng->nInputs);
  printf("Inputs: ");
  for (int i = 0; i < ng->nInputs; i++) {
    printf("%d ", ng->inputs[i]);
  }
  printf("\n");

  printf("Current Value: %s\n", ng->value ? "true" : "false");
  printf("Next Value: %s\n", ng->nextValue ? "true" : "false");
}
