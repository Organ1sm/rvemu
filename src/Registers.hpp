#pragma once

#include "RVEmu.hpp"

#include <array>
#include <cassert>
#include <string>

namespace rvemu
{
    class Registers
    {
      public:
        static constexpr std::array<std::string, RegistersNumber> RVABI = {
          "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
          "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
          "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
        };
        static std::string getABIName(std::size_t index);

      public:
        Registers() : registers_ {0}
        {
            registers_[RegisterIndex::Zero] = 0;
            registers_[RegisterIndex::SP]   = DRAM_END;
        }

        void write(std::size_t regIdx, RegisterSizeType what)
        {
            assert(regIdx < RegistersNumber);
            if (regIdx != RegisterIndex::Zero)
                registers_[regIdx] = what;
        }

        RegisterSizeType read(std::size_t regIdx) const { return registers_[regIdx]; }

      private:
        RegType registers_;
    };
}    // namespace rvemu
