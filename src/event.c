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
