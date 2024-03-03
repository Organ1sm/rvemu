#include "BitsManipulation.hpp"
#include "RVEmu.hpp"
#include <cassert>

namespace rvemu
{
    constexpr uint8_t BitsManipulation::lastIndexRegData()
    {
        return sizeof(RegisterSizeType) * 8 - 1;
    }

    auto BitsManipulation::takeBits(InstSizeType is, uint8_t begin, uint8_t end)
        -> RegisterSizeType
    {
        assert(end >= begin);
        return (is >> begin) &
               (static_cast<RegisterSizeType>(-1) >> (lastIndexRegData() - end + begin));
    }

    RegisterSizeType BitsManipulation::extendSign(const RegisterSizeType imm,
                                                  const uint8_t signPos)
    {
        assert(signPos <= lastIndexRegData());
        if ((imm >> signPos == 1) && (signPos < lastIndexRegData()))
            return -((1 << (signPos + 1)) - imm);
        return imm;
    }
}    // namespace rvemu
