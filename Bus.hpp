#pragma once

#include "DRAM.hpp"
#include <cstdint>
#include <optional>
#include <vector>

namespace rvemu
{
    class Bus
    {
      public:
        Bus(const std::vector<uint8_t> &code) : dram_(code) {}

        std::optional<uint64_t> load(uint64_t addr, uint64_t size);

        bool store(uint64_t addr, uint64_t size, uint64_t value);

      private:
        DRAM dram_;
    };
}    // namespace rvemu
