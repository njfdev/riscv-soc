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
  for (int i = 0; i < 8; i++) {
    printf("\tx%i: \t%08x (%i)\n", i, soc->rootp->soc__DOT__cpu0__DOT__regFile[i], soc->rootp->soc__DOT__cpu0__DOT__regFile[i]);
  }
}

int main() {
  auto soc = std::make_unique<Vsoc>();

  // basic assembly program
  soc->rootp->soc__DOT__ram0__DOT__ram[0] = 0b000000010010'00000'000'00010'0010011;   // ADDI x2, x0, 17 ; Load 0x12 into reg x2
  soc->rootp->soc__DOT__ram0__DOT__ram[1] = 0b000000000011'00000'000'00011'0010011;   // ADDI x3, x0, 3  ; Load 0x03 into reg x3
  soc->rootp->soc__DOT__ram0__DOT__ram[2] = 0b000000000110'00011'000'00011'0010011;   // ADDI x3, x3, 6  ; Add 0x06 to x3 and save back to x3
  soc->rootp->soc__DOT__ram0__DOT__ram[3] = 0b0000000'00011'00010'000'00100'0110011;  // ADD x4, x2, x3  ; Add x2 and x3 into x4 (should be 0x1b)
  soc->rootp->soc__DOT__ram0__DOT__ram[4] = 0b11111111000111111111'00001'1101111;     // JAL x1, -16     ; Start from beginning and save return address to x1 (ra)

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