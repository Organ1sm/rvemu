#include "Jformat.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"

namespace rvemu
{
    AddrType Jal::takeOffset()
    {
        u8 pos = 20;

        auto value = BitsManipulation::takeBits(inst_, 21, 30) << 1    // implicit shift done here
                     | BitsManipulation::takeBits(inst_, 20, 20) << 11
                     | BitsManipulation::takeBits(inst_, 12, 19) << 12
                     | BitsManipulation::takeBits(inst_, 31, 31) << 20;

        return BitsManipulation::extendSign(value, pos);
    }

    std::size_t Jal::takeIndexRd() { return BitsManipulation::takeBits(inst_, 7, 11); }

    void Jal::execution() { }

    void Jal::writeBack(Registers &regs) { regs.write(rdIdx_, currPC_ + DataSizeType::Word); }

    AddrType Jal::moveNextInst()
    {
        AddrType nextInst = currPC_ + offset_;
        return nextInst;
    }
}    // namespace rvemu
