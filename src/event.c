#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "event.h"
#include "nand.h"
#include "render.h"
#include "serialization.h"
#include "subassembly.h"
#include "types.h"
#include "util.h"

extern Vec2 pan;
extern float zoom;
extern NandGate *nandDrag;
extern Vec2 panDragStart;
extern bool isDraggingViewport;
extern Subassembly *subassemblyDrag;
extern Subassembly *subassemblyResize;
extern Vec2 clickPos;
extern NandGate **gates;
extern int nGates;
extern Subassembly **subassemblies;
extern int nSubassemblies;
extern Subassembly *currentSubassembly;
extern NandGate *selectedNand;

typedef enum MODE {
  NORMAL,
  SELECT_CHILD
}MODE;

MODE mode;

Port selection[2];

bool handleLeftMouseEvents(SDL_Event e, Vec2 mouse) {
  bool shouldRender=false;

  if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
    panDragStart.x = -1;
    panDragStart.y = -1;
    clickPos.x = mouse.x;
    clickPos.y = mouse.y;

    if(mode==SELECT_CHILD) {
      mode=NORMAL;
      for (int i = 0; i < nSubassemblies; i++) {
        float x = mouse.x;
        float y = mouse.y;
        Subassembly *s = subassemblies[i];
        clickPos.x = s->rect.x - x;
        clickPos.y = s->rect.y - y;
        if (isInSubassembly(s, x, y)) {
          if (subassemblies[i]!=currentSubassembly) {
            addSubassembly(currentSubassembly, i);
          }
          break;
        }
      }
    } else {
      bool subassemblyClicked = false;
      bool nandClicked = false;

      for (int i = 0; i < nGates; i++) {
        float x = mouse.x;
        float y = mouse.y;
        NandGate *g = gates[i];
        clickPos.x = g->pos.x - x;
        clickPos.y = g->pos.y - y;
        if (checkGateCollision(g, (Vec2){x, y})) {
          nandDrag = g;
          nandClicked = true;
          shouldRender = true;
          selectedNand = g;
          break;
        }
      }

      if (!nandClicked) {
        for (int i = 0; i < nSubassemblies; i++) {
          float x = mouse.x;
          float y = mouse.y;
          Subassembly *s = subassemblies[i];
          clickPos.x = s->rect.x - x;
          clickPos.y = s->rect.y - y;
          if (isInSubassembly(s, x, y)) {
            subassemblyDrag = s;
            subassemblyClicked = true;
            shouldRender = true;
            currentSubassembly = s;
            break;
          }
        }
      }

      if (!subassemblyClicked && !nandClicked) {
        isDraggingViewport = true;
      }
    }
  }

  if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
    subassemblyDrag = NULL;
    nandDrag = NULL;
    isDraggingViewport = false;
    shouldRender = true;
  }

  return shouldRender;
}
