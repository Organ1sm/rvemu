#include "DRAM.hpp"
#include "RVEmu.h"
#include <cstdint>
#include <stdexcept>


uint64_t DRAM::load(uint64_t addr, uint64_t size)
{
    if (size != 8 && size != 16 && size != 32 && size != 64)
        throw std::runtime_error("LoadAccessFault");

    uint64_t nbytes   = size / 8;
    std::size_t index = (addr - DRAM_BASE);

    if (index + nbytes > dram_.size())
        throw std::out_of_range("Address out of range");

    uint64_t value = 0;
    for (uint64_t i = 0; i < nbytes; i++)
        value |= static_cast<uint64_t>(dram_[index + i]) << (i * 8);

    return value;
}

void DRAM::store(uint64_t addr, uint64_t size, uint64_t value)
{
    if (size != 8 && size != 16 && size != 32 && size != 64)
        throw std::runtime_error("StoreAMQAccessFault");

    uint64_t nbytes   = size / 8;
    std::size_t index = (addr - DRAM_BASE);

    if (index + nbytes > dram_.size())
        throw std::out_of_range("Address out of range");

    for (uint64_t i = 0; i < nbytes; i++)
        dram_[index + i] = (value >> (i * 8)) & 0xFF; 
}
