#pragma once

#include "Cpu.hpp"

namespace rvemu
{
    class Emulator
    {
      public:
        Emulator(const std::string &);
        void runEmulator();

        const CPU &getCPU() { return cpu_; }

      private:
        CPU cpu_;
    };
}    // namespace rvemu
