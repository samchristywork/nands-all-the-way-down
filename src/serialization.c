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

void serializeString(FILE *f, const char *str) {
  if (str != NULL) {
    int length = strlen(str) + 1;
    fwrite(&length, sizeof(int), 1, f);
    fwrite(str, sizeof(char), length, f);
  } else {
    int length = 0;
    fwrite(&length, sizeof(int), 1, f);
  }
}

void serializeStringArray(FILE *f, char **strArray, int count) {
  for (int i = 0; i < count; ++i) {
    serializeString(f, strArray[i]);
  }
}

void serializeSubassembly(FILE *f, Subassembly *s) {
  serializeString(f, s->name);

  fwrite(&s->rect, sizeof(Rect), 1, f);

  fwrite(&s->nGates, sizeof(int), 1, f);
  if (s->nGates > 0) {
    fwrite(s->gates, sizeof(int), s->nGates, f);
  }

  fwrite(&s->nInputs, sizeof(int), 1, f);
  fwrite(&s->nOutputs, sizeof(int), 1, f);

  serializeStringArray(f, s->inputNames, s->nInputs);

  serializeStringArray(f, s->outputNames, s->nOutputs);

  fwrite(&s->nSubassemblies, sizeof(int), 1, f);
  if (s->nSubassemblies > 0) {
    fwrite(s->subassemblies, sizeof(int), s->nSubassemblies, f);
  }
}

char *deserializeString(FILE *f) {
  int length;
  fread(&length, sizeof(int), 1, f);
  if (length > 0) {
    char *str = (char *)malloc(length);
    fread(str, sizeof(char), length, f);
    return str;
  } else {
    return NULL;
  }
}

void deserializeStringArray(FILE *f, char ***strArray, int count) {
  *strArray = (char **)malloc(count * sizeof(char *));
  for (int i = 0; i < count; ++i) {
    (*strArray)[i] = deserializeString(f);
  }
}

void deserializeSubassembly(FILE *f, Subassembly *s) {
  s->name = deserializeString(f);

  fread(&s->rect, sizeof(Rect), 1, f);

  fread(&s->nGates, sizeof(int), 1, f);
  if (s->nGates > 0) {
    s->gates = (int *)malloc(s->nGates * sizeof(int));
    fread(s->gates, sizeof(int), s->nGates, f);
  } else {
    s->gates = NULL;
  }

  fread(&s->nInputs, sizeof(int), 1, f);
  fread(&s->nOutputs, sizeof(int), 1, f);

  deserializeStringArray(f, &s->inputNames, s->nInputs);
  deserializeStringArray(f, &s->outputNames, s->nOutputs);

  fread(&s->nSubassemblies, sizeof(int), 1, f);
  if (s->nSubassemblies > 0) {
    s->subassemblies = (int *)malloc(s->nSubassemblies * sizeof(int));
    fread(s->subassemblies, sizeof(int), s->nSubassemblies, f);
  } else {
    s->subassemblies = NULL;
  }
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

void load() {
  printf("Loading\n");

  FILE *f = fopen("save", "r");
  if (!f) {
    return;
  }

  fread(&nSubassemblies, sizeof(nSubassemblies), 1, f);
  subassemblies = malloc(sizeof(Subassembly *) * nSubassemblies);

  for (int i = 0; i < nSubassemblies; i++) {
    subassemblies[i] = malloc(sizeof(Subassembly));
  }

  for (int i = 0; i < nSubassemblies; i++) {
    deserializeSubassembly(f, subassemblies[i]);
  }

  fread(&nGates, sizeof(nGates), 1, f);
  gates = malloc(sizeof(NandGate *) * nGates);

  for (int i = 0; i < nGates; i++) {
    gates[i] = malloc(sizeof(NandGate));
    gates[i]->shade = 255;
    gates[i]->portHighlight = -1;
  }

  for (int i = 0; i < nGates; i++) {
    deserializeNand(f, gates[i]);
  }

  fclose(f);
}
