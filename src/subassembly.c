#include "subassembly.h"
#include "nand.h"
#include "render.h"
#include "types.h"

extern NandGate **gates;
extern int nGates;

extern Subassembly **subassemblies;
extern int nSubassemblies;

Vec2 getInputsScreenPos(Subassembly *s, int n, Vec2 pan, float zoom) {
  float x = s->rect.x - pan.x;
  float y = s->rect.y - pan.y;

  float pitch = s->rect.h / s->nInputs;
  return (Vec2){x, y + (float)n * pitch + pitch / 2};
}

Vec2 getOutputsScreenPos(Subassembly *s, int n, Vec2 pan, float zoom) {
  float x = s->rect.x - pan.x + s->rect.w;
  float y = s->rect.y - pan.y;

  float pitch = s->rect.h / s->nOutputs;
  return (Vec2){x, y + (float)n * pitch + pitch / 2};
}

void drawSubassembly(SDL_Renderer *renderer, TTF_Font *font, Vec2 pan, float zoom,
                     Subassembly *subassembly) {
  float x = subassembly->rect.x - pan.x;
  float y = subassembly->rect.y - pan.y;

  //box(renderer, pan, zoom, x, y, x + subassembly->rect.w, y + subassembly->rect.h, 100,
  //        100, 100, 255);
  rect(renderer, pan, zoom, x, y, x + subassembly->rect.w,
                y + subassembly->rect.h, 255, 255, 255, 255);

  if (strlen(subassembly->name) > 0) {
    renderText(renderer, font, x*zoom, y*zoom, subassembly->name);
  } else {
    renderText(renderer, font, x*zoom, y*zoom, "(No Name)");
  }

  for (int i = 0; i < subassembly->nInputs; i++) {
    Vec2 p = getInputsScreenPos(subassembly, i, pan, zoom);

    renderText(renderer, font, p.x*zoom, p.y*zoom, subassembly->inputNames[i]);
  }

  for (int i = 0; i < subassembly->nOutputs; i++) {
    Vec2 p = getOutputsScreenPos(subassembly, i, pan, zoom);

    renderText(renderer, font, p.x*zoom, p.y*zoom, subassembly->outputNames[i]);
  }

  for (int i = 0; i < subassembly->nSubassemblies; i++) {
    Subassembly *child=subassemblies[subassembly->subassemblies[i]];
    thickLineRGBA(renderer, x*zoom, y*zoom, (child->rect.x-pan.x)*zoom, (child->rect.y-pan.y)*zoom, 1, 255, 255, 255, 255);
  }
}

Subassembly *createSubassembly(char *name, Rect rect, int nInputs, int nOutputs,
                               char **inputNames, char **outputNames) {
  Subassembly *s = malloc(sizeof(Subassembly));
  s->name = name;
  s->rect.x = rect.x;
  s->rect.y = rect.y;
  s->rect.w = rect.w;
  s->rect.h = rect.h;
  s->nInputs = nInputs;
  s->nOutputs = nOutputs;
  s->inputNames = inputNames;
  s->outputNames = outputNames;
  s->nGates = 0;
  s->nSubassemblies = 0;

  if (subassemblies == NULL) {
    subassemblies = malloc(sizeof(Subassembly *));
  } else {
    subassemblies =
        realloc(subassemblies, sizeof(Subassembly *) * (nSubassemblies + 1));
  }

  subassemblies[nSubassemblies] = s;

  nSubassemblies++;

  return s;
}

void addInput(Subassembly *s, char *name) {
  if (s == NULL) {
    return;
  }

  if (s->nInputs == 0) {
    s->nInputs = 1;
    s->inputNames = malloc(sizeof(char *));
  } else {
    s->nInputs++;
    s->inputNames = realloc(s->inputNames, sizeof(char *) * s->nInputs);
  }

  s->inputNames[s->nInputs - 1] = malloc(strlen(name));
  strcpy(s->inputNames[s->nInputs - 1], name);
}

void addOutput(Subassembly *s, char *name) {
  if (s == NULL) {
    return;
  }

  if (s->nOutputs == 0) {
    s->nOutputs = 1;
    s->outputNames = malloc(sizeof(char *));
  } else {
    s->nOutputs++;
    s->outputNames = realloc(s->outputNames, sizeof(char *) * s->nOutputs);
  }

  s->outputNames[s->nOutputs - 1] = malloc(strlen(name));
  strcpy(s->outputNames[s->nOutputs - 1], name);
}
