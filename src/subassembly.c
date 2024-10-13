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

void subAssemblyAddGate(Subassembly *s, int g) {
  if (s == NULL || g < 0) {
    return;
  }

  if (s->nGates == 0) {
    s->gates = malloc((s->nGates + 1) * sizeof(int));
  } else {
    s->gates = realloc(s->gates, (s->nGates + 1) * sizeof(int));
  }

  if (s->gates == NULL) {
    perror("Failed to add gate due to memory allocation failure");
    exit(EXIT_FAILURE);
  }

  s->gates[s->nGates] = g;

  s->nGates++;
}

bool isInSubassembly(Subassembly *s, float x, float y) {
  if (x > s->rect.x && x < s->rect.x + s->rect.w) {
    if (y > s->rect.y && y < s->rect.y + s->rect.h) {
      return true;
    }
  }
  return false;
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

void addSubassembly(Subassembly *s, int i) {
  printf("Adding %s to %s\n", subassemblies[i]->name, s->name);
  if (!s) {
    return;
  }

  if (i<0) {
    return;
  }

  if (s->nSubassemblies==0) {
    s->subassemblies=malloc(sizeof(int));
    s->subassemblies[0]=i;
    s->nSubassemblies=1;
  } else {
    s->subassemblies=realloc(s->subassemblies, sizeof(int)*(s->nSubassemblies+1));
    s->subassemblies[s->nSubassemblies]=i;
    s->nSubassemblies++;
  }
}

void printSubassembly(Subassembly *s) {
  if (!s) {
    printf("Subassembly is NULL\n");
    return;
  }

  printf("Subassembly Name: %s\n", s->name);
  printf("Rectangle: x=%f, y=%f, w=%f, h=%f\n", s->rect.x, s->rect.y, s->rect.w,
         s->rect.h);

  printf("Gates (%d): ", s->nGates);
  for (int i = 0; i < s->nGates; i++) {
    printf("%d ", s->gates[i]);
  }
  printf("\n");

  printf("Number of Inputs: %d\n", s->nInputs);
  printf("Input Names:\n");
  for (int i = 0; i < s->nInputs; i++) {
    printf("  - %s\n", s->inputNames[i]);
  }

  printf("Number of Outputs: %d\n", s->nOutputs);
  printf("Output Names:\n");
  for (int i = 0; i < s->nOutputs; i++) {
    printf("  - %s\n", s->outputNames[i]);
  }

  printf("Subassemblies (%d): ", s->nSubassemblies);
  for (int i = 0; i < s->nSubassemblies; i++) {
    printf("%d ", s->subassemblies[i]);
  }
  printf("\n");
}

int copySubassembly(Subassembly *cs) {
  char *name = malloc(strlen(cs->name));
  strcpy(name, cs->name);
  Rect rect;
  memcpy(&rect, &cs->rect, sizeof(Rect));
  rect.x += 100;
  rect.y += 100;
  Subassembly *s = createSubassembly(name, rect, 0, 0, NULL, NULL);
  int ret = nSubassemblies-1;

  s->nInputs = cs->nInputs;
  s->nOutputs = cs->nOutputs;
  s->inputNames = malloc(sizeof(int) * s->nInputs);
  s->outputNames = malloc(sizeof(int) * s->nOutputs);

  for (int i = 0; i < s->nInputs; i++) {
    s->inputNames[i] =
        malloc(strlen(cs->inputNames[i]));
    strcpy(s->inputNames[i], cs->inputNames[i]);
  }

  for (int i = 0; i < s->nOutputs; i++) {
    s->outputNames[i] =
        malloc(strlen(cs->outputNames[i]));
    strcpy(s->outputNames[i], cs->outputNames[i]);
  }

  NandGate **newGates =
      malloc(sizeof(NandGate *) * cs->nGates);
  Pair *map = malloc(sizeof(Pair) * cs->nGates);

  for (int i = 0; i < cs->nGates; i++) {
    NandGate *g = createNand(
        (Vec2){gates[cs->gates[i]]->pos.x + 100,
               gates[cs->gates[i]]->pos.y + 100},
        gates[cs->gates[i]]->nInputs);
    subAssemblyAddGate(s, nGates - 1);

    newGates[i] = g;
    map[i].from = cs->gates[i];
    map[i].to = nGates - 1;

    int nInputs = gates[cs->gates[i]]->nInputs;
    for (int j = 0; j < nInputs; j++) {
      g->inputs[j] = gates[cs->gates[i]]->inputs[j];
    }
  }

  for (int i = 0; i < cs->nGates; i++) {
    NandGate *g = newGates[i];
    for (int j = 0; j < g->nInputs; j++) {
      for (int k = 0; k < cs->nGates; k++) {
        if (map[k].from == g->inputs[j]) {
          g->inputs[j] = map[k].to;
        }
      }
    }
  }

  s->nSubassemblies=cs->nSubassemblies;
  s->subassemblies=malloc(sizeof(int)*s->nSubassemblies);
  for (int i = 0; i < cs->nSubassemblies; i++) {
    s->subassemblies[i]=copySubassembly(subassemblies[cs->subassemblies[i]]);
  }

  return ret;
}
