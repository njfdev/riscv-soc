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

void print_regs(std::unique_ptr<Vsoc>& soc) {
  printf("Regs:\n");
  for (int i = 0; i < 32; i++) {
    printf("\t%ix: %08x\n", i, soc->rootp->soc__DOT__cpu0__DOT__regFile[i]);
  }
}

int main() {
  auto soc = std::make_unique<Vsoc>();

  // basic assembly program
  soc->rootp->soc__DOT__ram0__DOT__ram[0] = 0b000000010010'00010'000'00011'0010011;   // ADDI x3, x2, 17
  soc->rootp->soc__DOT__ram0__DOT__ram[1] = 0b0000000'00011'00010'000'00001'0110011;  // ADD x1, x2, x3
  soc->rootp->soc__DOT__ram0__DOT__ram[2] = 0b11111111100111111111'00001'1101111;     // JAL x1, -8

  do {
    cycle(soc);
    printf("Program Counter: %08x\n", soc->rootp->soc__DOT__cpu0__DOT__pc);
    printf("\tData at PC: %08x\n", soc->rootp->soc__DOT__ram0__DOT__ram[soc->rootp->soc__DOT__cpu0__DOT__pc/4]);
    printf("\tInstr register: %08x\n", soc->rootp->soc__DOT__cpu0__DOT__instr);
    printf("Phase: %s\n", soc->rootp->soc__DOT__cpu0__DOT__state == 1 ? "execute" : "fetch");
    print_regs(soc);
  } while (std::cin.get() != 'q');

  soc->final();
}