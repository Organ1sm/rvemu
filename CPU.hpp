#pragma once

#include "Bus.hpp"
#include "CSR.hpp"
#include "RVEmu.h"
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>


namespace rvemu
{
    class CPU
    {
      public:
        CPU(std::vector<uint8_t> &code)
            : pc(DRAM_BASE), bus(code), csr(),
              RVABI {
                  "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
                  "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
                  "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",

              }
        {
            regs.fill(0);               // initialize the regs value to 0
            regs[2] = DRAM_SIZE - 1;    // set  the initial value for sp reg
        }


        // PC register
        uint64_t pc;

        std::array<uint64_t, 32> regs {};

        Bus bus;

        Csr csr;

        std::optional<uint64_t> load(uint64_t addr, uint64_t size);

        bool store(uint64_t addr, uint64_t size, uint64_t value);

        std::optional<uint32_t> fetch();

        [[nodiscard]] inline uint64_t updatePC() const { return pc + 4; }

        std::optional<uint64_t> execute(uint32_t inst);

        void dumpRegisters();
        void dumpCSRs();
        void dumpPC() const;

        std::optional<uint64_t> getRegValueByName(const std::string &regName);

      private:
        const std::array<std::string, 32> RVABI;
    };
}    // namespace rvemu
