#include <iostream>

#include "Vsoc.h"
#include "verilated.h"

int main() {
  auto soc = std::make_unique<Vsoc>();

  soc->clk = 1;
  soc->eval();
  std::cout << "Clock: " << (int)soc->clk << "\n";
  soc->clk = 0;
  soc->eval();
  std::cout << "Clock: " << (int)soc->clk << "\n";


  soc->final();
}