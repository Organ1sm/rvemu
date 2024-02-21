#include "DRAM.hpp"
#include "RVEmu.h"
#include <cstdint>
#include <optional>

namespace rvemu
{
    std::optional<uint64_t> DRAM::load(uint64_t addr, uint64_t size)
    {
        if (size != 8 && size != 16 && size != 32 && size != 64)
            return std::nullopt;

        uint64_t nbytes   = size / 8;
        std::size_t index = (addr - DRAM_BASE);

        if (index + nbytes > dram_.size())
            return std::nullopt;

        uint64_t value = 0;
        for (uint64_t i = 0; i < nbytes; i++)
            value |= static_cast<uint64_t>(dram_[index + i]) << (i * 8);

        return value;
    }

    bool DRAM::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        if (size != 8 && size != 16 && size != 32 && size != 64)
            return false;

        uint64_t nbytes   = size / 8;
        std::size_t index = (addr - DRAM_BASE);
        if (index + nbytes > dram_.size())
            return false;

        for (uint64_t i = 0; i < nbytes; i++)
            dram_[index + i] = (value >> (i * 8)) & 0xFF;

        return true;
    }
}    // namespace rvemu
