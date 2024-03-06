#pragma once

#include "InstFormat.hpp"

#include <iostream>

namespace rvemu
{
    class Fence : public InstructionFormat
    {
      public:
        Fence(const InstSizeType is, const AddrType pc) : InstructionFormat(is, pc) { }

        void execution() override { std::cout << "Fence instruction: synchronization\n"; }
    };
}    // namespace rvemu
