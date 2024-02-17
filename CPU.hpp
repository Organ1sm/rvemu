#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

const uint64_t DRAM_SIZE = 1024 * 1024 * 128;

class CPU {
public:
  CPU(std::vector<uint8_t> &code) : pc(0), dram(code) {
    regs.fill(0);            // initialize the regs value to 0
    regs[2] = DRAM_SIZE - 1; // set  the initial value for sp reg
  }

  std::array<uint64_t, 32> regs;
  uint64_t pc;
  std::vector<uint8_t> dram;

  const std::array<std::string, 32> RVABI = {
      "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
      "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
      "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",

  };

  uint32_t fetch();
  void execute(uint32_t inst);
  void dumpRegisters();
};
