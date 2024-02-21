#pragma once

#include "CPU.hpp"
#include <cstdint>
#include <optional>

namespace rvemu
{

    class InstructionExecutor
    {
      public:
        static std::optional<uint64_t> execute(CPU &cpu, uint32_t inst);
    };
};    // namespace rvemu
