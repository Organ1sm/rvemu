#pragma once

#include "CPU.hpp"
#include <cstdint>
#include <functional>
#include <optional>

namespace rvemu
{

    class InstructionExecutor
    {
        using ExecuteFunction = std::function<std::optional<uint64_t>(CPU &, uint32_t)>;

      public:
        static std::optional<uint64_t> execute(CPU &cpu, uint32_t inst);
    };
};    // namespace rvemu
