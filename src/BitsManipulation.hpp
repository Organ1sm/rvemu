#pragma once

#include "RVEmu.hpp"

namespace rvemu
{
    struct BitsManipulation
    {
        constexpr static u8 lastIndexRegData();
        static RegisterSizeType takeBits(InstSizeType,
                                         u8 begin,
                                         u8 end);    // Take bits from begin to end INCLUDED
        static RegisterSizeType extendSign(RegisterSizeType, u8);
    };
}    // namespace rvemu
