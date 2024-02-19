#include "Bus.hpp"
#include "RVEmu.h"
#include <cstdint>
#include <stdexcept>
#include <string>

uint64_t Bus::load(uint64_t addr, uint64_t size)
{
    if (addr >= DRAM_BASE && addr <= DRAM_END)
        return dram_.load(addr, size);
    else
        throw std::runtime_error("LoadAccessFault at address " + std::to_string(addr));
}

void Bus::store(uint64_t addr, uint64_t size, uint64_t value)
{
    if (addr >= DRAM_BASE && addr <= DRAM_END)
        dram_.store(addr, size, value);
    else
        throw std::runtime_error("StoreAccessFault at address " + std::to_string(addr));
}
