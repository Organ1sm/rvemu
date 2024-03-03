#pragma once

#include <cstdint>

#include "RVEmu.hpp"

namespace rvemu
{
    struct BitsManipulation
    {
        constexpr static uint8_t lastIndexRegData();
        static RegisterSizeType
            takeBits(InstSizeType,
                     uint8_t begin,
                     uint8_t end);    // Take bits from begin to end INCLUDED
        static RegisterSizeType extendSign(RegisterSizeType, uint8_t);
    };
}    // namespace rvemu
