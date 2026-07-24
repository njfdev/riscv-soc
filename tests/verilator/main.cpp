#include <cstdio>
#include <iostream>
#include <memory>

#include "Vsoc.h"
#include "Vsoc___024root.h"
#include "verilated.h"

void cycle(std::unique_ptr<Vsoc>& soc) {
  soc->clk = 1;
  soc->eval();
  soc->clk = 0;
  soc->eval();
}

int main() {
  auto soc = std::make_unique<Vsoc>();

  // set 2 memory addresses
  soc->rootp->soc__DOT__addrBus = 1;
  soc->rootp->soc__DOT__tbData = 0xDEADBEEF;
  soc->rootp->soc__DOT__tbDataEnable = true;
  soc->rootp->soc__DOT__memWrite = true;
  printf("Setting 0x%08x to 0x%08x\n", soc->rootp->soc__DOT__addrBus, soc->rootp->soc__DOT__tbData);
  cycle(soc);
  soc->rootp->soc__DOT__addrBus = 2;
  soc->rootp->soc__DOT__tbData = 0x12345678;
  printf("Setting 0x%08x to 0x%08x\n", soc->rootp->soc__DOT__addrBus, soc->rootp->soc__DOT__tbData);
  cycle(soc);

  // read the 2 memory addresses and the 0 address
  soc->rootp->soc__DOT__addrBus = 0;
  soc->rootp->soc__DOT__tbDataEnable = false;
  soc->rootp->soc__DOT__memWrite = false;
  soc->rootp->soc__DOT__memRead = true;
  cycle(soc);
  std::printf("Reading at 0x%08x which is 0x%08x\n", soc->rootp->soc__DOT__addrBus, soc->rootp->soc__DOT__dataBus);
  soc->rootp->soc__DOT__addrBus = 1;
  cycle(soc);
  std::printf("Reading at 0x%08x which is 0x%08x\n", soc->rootp->soc__DOT__addrBus, soc->rootp->soc__DOT__dataBus);
  soc->rootp->soc__DOT__addrBus = 2;
  cycle(soc);
  std::printf("Reading at 0x%08x which is 0x%08x\n", soc->rootp->soc__DOT__addrBus, soc->rootp->soc__DOT__dataBus);


  soc->final();
}