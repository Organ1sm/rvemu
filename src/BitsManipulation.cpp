#include "BitsManipulation.hpp"

#include "RVEmu.hpp"

#include <cassert>

namespace rvemu
{
    constexpr u8 BitsManipulation::lastIndexRegData() { return sizeof(RegisterSizeType) * 8 - 1; }

    RegisterSizeType BitsManipulation::takeBits(InstSizeType is, u8 begin, u8 end)
    {
        assert(end >= begin);
        return (is >> begin)
               & (static_cast<RegisterSizeType>(-1) >> (lastIndexRegData() - end + begin));
    }

    RegisterSizeType BitsManipulation::extendSign(const RegisterSizeType imm, const u8 signPos)
    {
        assert(signPos <= lastIndexRegData());
        if ((imm >> signPos == 1) && (signPos < lastIndexRegData()))
            return -((1 << (signPos + 1)) - imm);
        return imm;
    }
}    // namespace rvemu
