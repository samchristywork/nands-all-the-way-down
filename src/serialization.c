#include "nand.h"
#include "subassembly.h"

extern Subassembly **subassemblies;
extern int nSubassemblies;

extern NandGate **gates;
extern int nGates;

void serializeNand(FILE *f, NandGate *g) {
  fwrite(&g->pos, sizeof(Vec2), 1, f);

  fwrite(&g->nInputs, sizeof(int), 1, f);

  if (g->nInputs > 0) {
    fwrite(g->inputs, sizeof(int), g->nInputs, f);
  }

  fwrite(&g->value, sizeof(bool), 1, f);
  fwrite(&g->nextValue, sizeof(bool), 1, f);
}

void deserializeNand(FILE *f, NandGate *g) {
  fread(&g->pos, sizeof(Vec2), 1, f);

  fread(&g->nInputs, sizeof(int), 1, f);

  if (g->nInputs > 0) {
    g->inputs = (int *)malloc(g->nInputs * sizeof(int));
    fread(g->inputs, sizeof(int), g->nInputs, f);
  } else {
    g->inputs = NULL;
  }

  fread(&g->value, sizeof(bool), 1, f);
  fread(&g->nextValue, sizeof(bool), 1, f);
}

void save() {
  printf("Saving\n");

  FILE *f = fopen("save", "w");

  fwrite(&nSubassemblies, sizeof(nSubassemblies), 1, f);
  for (int i = 0; i < nSubassemblies; i++) {
    serializeSubassembly(f, subassemblies[i]);
  }

  fwrite(&nGates, sizeof(nGates), 1, f);
  for (int i = 0; i < nGates; i++) {
    serializeNand(f, gates[i]);
  }

  fclose(f);
}
