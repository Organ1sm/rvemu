#pragma once

#include "DRAM.hpp"
#include <cstdint>
#include <vector>

class Bus
{
  public:
    Bus(const std::vector<uint8_t> &code) : dram_(code) {}

    uint64_t load(uint64_t addr, uint64_t size);

    void store(uint64_t addr, uint64_t size, uint64_t value);


  private:
    DRAM dram_;
};
