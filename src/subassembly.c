#include "subassembly.h"
#include "nand.h"
#include "render.h"
#include "types.h"

extern NandGate **gates;
extern int nGates;

extern Subassembly **subassemblies;
extern int nSubassemblies;

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
