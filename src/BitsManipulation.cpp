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
        // Ensure the sign position is within the bounds of RegisterSizeType.
        assert(signPos <= lastIndexRegData());

        // Check if the sign bit at signPos is set (i.e., equals 1).
        // Also, make sure the sign position is not the last bit to avoid overflow.
        if ((imm >> signPos & 1) == 1 && (signPos < lastIndexRegData()))
        {
            // If the sign bit is set, we perform sign extension by creating a mask of 1's up to
            // the sign position, subtracting the immediate value from it, then negating the
            // result to extend the sign bit to the left.
            return -((1 << (signPos + 1)) - imm);
        }

        // If the sign bit is not set or signPos is the last bit, return the immediate value
        // unchanged.
        return imm;
    }
}    // namespace rvemu
