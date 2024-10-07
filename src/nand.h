#ifndef NAND_H
#define NAND_H

#include "types.h"

void tick();

int portFromPos(NandGate *g, float x, float y);

NandGate *createNand(Vec2 pos, int n);

#endif
