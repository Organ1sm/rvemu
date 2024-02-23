#include <cstdint>
#include <optional>
#include "Bus.hpp"
#include "RVEmu.h"
#include "Log.hpp"

namespace rvemu
{
    std::optional<uint64_t> Bus::load(uint64_t addr, uint64_t size)
    {
        if (addr >= DRAM_BASE && addr <= DRAM_END)
        {
            LOG(INFO,
                fmt::format("Bus loading from DRAM address {:#x} with size {} bytes ",
                            addr,
                            size));

            return dram_.load(addr, size);
        }

        return std::nullopt;
    }

    bool Bus::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        if (addr >= DRAM_BASE && addr <= DRAM_END)
        {
            LOG(INFO,
                fmt::format(
                    "Bus storing value  {:#x} at DRAM address {:#x} with size {} bytes.",
                    value,
                    addr,
                    size));

            return dram_.store(addr, size, value);
        }
        else
        {
            LOG(ERROR,
                fmt::format(
                    "Invalid address for store operation: {:#x}. Valid address range is [{:#x}, {:#x}] ",
                    addr,
                    DRAM_BASE,
                    DRAM_END));

            return false;
        }
    }
}    // namespace rvemu
