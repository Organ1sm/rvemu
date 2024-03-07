#include "Uformat.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"

namespace rvemu
{
    RegisterSizeType U::takeOffset()
    {
        return BitsManipulation::extendSign(BitsManipulation::takeBits(inst_, 12, 31), 19) << 12;
    }

    size_t U::takeRd() { return BitsManipulation::takeBits(inst_, 7, 11); }

    void U::writeBack(Registers &regs) { regs.write(rdIdx_, rd_); }

    void Auipc::execution() { rd_ = offset_ + currPC_; }

    void Lui::execution() { rd_ = offset_; }
}    // namespace rvemu
