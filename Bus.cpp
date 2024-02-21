#include <cstdint>
#include <optional>
#include "Bus.hpp"
#include "RVEmu.h"

namespace rvemu
{
    std::optional<uint64_t> Bus::load(uint64_t addr, uint64_t size)
    {
        if (addr >= DRAM_BASE && addr <= DRAM_END)
            return dram_.load(addr, size);

        return std::nullopt;
    }

    bool Bus::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        if (addr >= DRAM_BASE && addr <= DRAM_END)
            return dram_.store(addr, size, value);
        else
            return false;
    }
}    // namespace rvemu
