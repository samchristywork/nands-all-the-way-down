#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef struct {
  float x;
  float y;
} Vec2;

typedef struct {
  float x;
  float y;
  float w;
  float h;
} Rect;

typedef struct Subassembly {
  char *name;
  Rect rect;
  int *gates;
  int nGates;
  int nInputs;
  int nOutputs;
  char **inputNames;
  char **outputNames;
  int *subassemblies;
  int nSubassemblies;
} Subassembly;

typedef struct NandGate {
  Vec2 pos;
  int *inputs;
  int nInputs;
  bool value;
  bool nextValue;
  float shade;
  int portHighlight;
} NandGate;

typedef struct Pair {
  int from;
  int to;
} Pair;

#endif
