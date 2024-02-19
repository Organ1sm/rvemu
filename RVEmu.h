#pragma once

#include <cstddef>

constexpr std::size_t DRAM_BASE = 0x8000'0000;

// define the size of DRAM => 128MB
constexpr std::size_t DRAM_SIZE = 1024 * 1024 * 128;

constexpr std::size_t DRAM_END = DRAM_BASE + DRAM_SIZE - 1;
