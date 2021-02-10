#include <stdio.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <iostream>

#include "loader.h"

int main(int argc, char** argv)
{
  size_t i;
  Binary bin;
  Section *sec;
  Symbol *sym;
  std::string fname;
  std::string secOpt = std::string();

  if(argc < 2) {
    printf("%s [FILE] [OPTION]\n\n", argv[0]);
    printf("%-3s-h [SECTION]\n", "");
    printf("%-7sprint section in hexadecimal format\n", "");
    return 1;
  }

  for(int j = 0; j < argc; ++j){
    if (!strcmp(argv[j], "-h") && (j + 1) < argc){
      printf("-h found: %s\n", argv[j]);
      secOpt = argv[j + 1];
    }
  }


  fname.assign(argv[1]);
  std::cout << "loading binary" << std::endl;
  if(load_binary(fname, &bin, Binary::BIN_TYPE_AUTO) < 0)
    return 1;

  printf("Loaded binary '%s' %s/%s (%u bits) entry@0x%016jx\n",
      bin.filename.c_str(), bin.type_str.c_str(), bin.arch_str.c_str(),
      bin.bits, bin.entry);

  for(i = 0; i < bin.sections.size(); ++i) {
    sec = &bin.sections[i];
    printf(" 0x%016jx %-8ju %-20s %s\n", sec->vma, sec->size, sec->name.c_str(),
        sec->type == Section::SEC_TYPE_CODE ? "CODE" : "DATA");
  }


  sec = bin.get_section(secOpt.c_str());
  if (sec) {
    printf("Dumping %s section:\n", sec->name.c_str());
    unsigned char ascii[17];
    uint64_t p;

    for (p = 0; p < sec->size; ++p) {
      if (( p % 16) == 0) {
        if (p != 0)
          printf(" %s\n", ascii);
        printf(" %04lx ", p + sec->vma);
      }

      printf("%02x ", sec->bytes[p]);

      if ((sec->bytes[p] < 0x20) || (sec->bytes[p] > 0x7e))
        ascii[p % 16] = '.';
      else
        ascii[p % 16] = sec->bytes[p];
      ascii[(p % 16) + 1] = '\0';
    }
    while ((p % 16) !=0) {
      printf("   ");
      ++p;
    }

    printf(" %s\n", ascii);
  }
  else
    printf("%s section not found\n", secOpt.c_str());



  if(bin.symbols.size() > 0) {
    printf("scanned symbol tables\n");
    for(i = 0; i < bin.symbols.size(); ++i) {
      sym = &bin.symbols[i];
      printf(" %-40s 0x%016jx %s\n", sym->name.c_str(), sym->addr,
          (sym->type & Symbol::SYM_TYPE_FUNC) ? "FUNC" : "");
    }
  }

  unload_binary(&bin);

  return 0;
}
