#include "DRAM.hpp"
#include "Log.hpp"
#include "RVEmu.h"
#include <cstdint>
#include <optional>

namespace rvemu
{
    std::optional<uint64_t> DRAM::load(uint64_t addr, uint64_t size)
    {
        if (size != 8 && size != 16 && size != 32 && size != 64)
        {
            LOG(WARNING, fmt::format("Invalid size for load operation: {} bytes.", size));
            return std::nullopt;
        }

        uint64_t nbytes   = size / 8;
        std::size_t index = (addr - DRAM_BASE);
        if (index + nbytes > dram_.size())
        {
            LOG(WARNING,
                fmt::format("Invalid address range for load operation at DRAM address {}",
                            addr));
            return std::nullopt;
        }

        uint64_t value = 0;
        for (uint64_t i = 0; i < nbytes; i++)
            value |= static_cast<uint64_t>(dram_[index + i]) << (i * 8);

        LOG(INFO, fmt::format("DRAM load successful. value: {:#x} ", value));
        return value;
    }

    bool DRAM::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        if (size != 8 && size != 16 && size != 32 && size != 64)
        {
            LOG(WARNING,
                fmt::format("Invalid size for store operation: {} bytes.", size));
            return false;
        }

        uint64_t nbytes   = size / 8;
        std::size_t index = (addr - DRAM_BASE);
        if (index + nbytes > dram_.size())
        {
            LOG(WARNING,
                fmt::format(
                    "Invalid address range for store operation at DRAM address {}",
                    addr));
            return false;
        }

        for (uint64_t i = 0; i < nbytes; i++)
            dram_[index + i] = (value >> (i * 8)) & 0xFF;

        LOG(INFO,
            fmt::format(
                "DRAM store successful. Value: {} at address {:#x} with size {} bytes. ",
                value,
                addr,
                size));
        return true;
    }
}    // namespace rvemu
