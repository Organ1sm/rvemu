#pragma once

#include "RVEmu.h"
#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>

namespace rvemu
{
    class DRAM
    {
      public:
        DRAM() = default;
        DRAM(const std::vector<uint8_t> &code)
        {
            dram_.resize(DRAM_SIZE, 0);
            std::copy(code.begin(), code.end(), dram_.begin());
        }

        std::optional<uint64_t> load(uint64_t addr, uint64_t size);

        bool store(uint64_t addr, uint64_t size, uint64_t value);

      private:
        std::vector<uint8_t> dram_;
    };
}    // namespace rvemu
