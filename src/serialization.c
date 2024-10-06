#include "nand.h"
#include "subassembly.h"

extern Subassembly **subassemblies;
extern int nSubassemblies;

extern NandGate **gates;
extern int nGates;

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
