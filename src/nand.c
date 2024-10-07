#include <stdbool.h>
#include <stdlib.h>

#include "nand.h"

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

int portFromPos(NandGate *g, float x, float y) {
  if (g->nInputs == 0) {
    return -1;
  } else if (g->nInputs == 1) {
    float dx = g->pos.x - x;
    float dy = g->pos.y - y;
    if (dx * dx + dy * dy < 25 * 25) {
      return 0;
    }
  } else {
    for (int i = 0; i < g->nInputs; i++) {
      float pitch = 30 / (g->nInputs - 1);

      float w = 10;
      float h = 10;
      if (x > g->pos.x - w && x < g->pos.x + w) {
        if (y > g->pos.y - 15 + pitch * i - h &&
            y < g->pos.y - 15 + pitch * i + h) {
          return i;
        }
      }
    }
  }

  return -1;
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
