#ifndef EVENT_H
#define EVENT_H

#include "nand.h"

typedef struct Port {
  NandGate *g;
  int id;
  int n;
  bool selected;
} Port;

#endif
