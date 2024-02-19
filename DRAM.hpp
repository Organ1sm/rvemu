#pragma once

#include "RVEmu.h"
#include <algorithm>
#include <cstdint>
#include <vector>

class DRAM
{
  public:
    DRAM(const std::vector<uint8_t> &code)
    {
        dram_.resize(DRAM_SIZE, 0);
        std::copy(code.begin(), code.end(), dram_.begin());
    }


    uint64_t load(uint64_t addr, uint64_t size);

    void store(uint64_t addr, uint64_t size, uint64_t value);

  private:
    std::vector<uint8_t> dram_;
};
