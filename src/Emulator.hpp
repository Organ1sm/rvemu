#pragma once

#include "Cpu.hpp"

namespace rvemu
{
    class Emulator
    {
      public:
        Emulator(const std::string &);
        void runEmulator();

      private:
        CPU cpu_;
    };
}    // namespace rvemu
