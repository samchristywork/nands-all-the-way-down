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

bool checkGateCollision(NandGate *g, float x, float y) {
  int gx = g->pos.x;
  int gy = g->pos.y;

  if (x > gx - 15 && x < gx + 55) {
    if (y > gy - 25 && y < gy + 25) {
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
