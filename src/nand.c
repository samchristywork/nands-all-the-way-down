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
