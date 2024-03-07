#include "Store.hpp"

#include "../BitsManipulation.hpp"
#include "../Memory.hpp"
#include "../Registers.hpp"

#include <iostream>

namespace rvemu
{
    void Store::readRegister(const Registers &reg)
    {
        rs1_ = reg.read(rs1Idx_);
        rs2_ = reg.read(rs2Idx_);
    }

    void Store::execution() { addrToWrite = rs1_ + offset_; }

    void Store::accessMemory(SystemInterface &bus)
    {
        if (func3_ > 2)
        {
            std::cerr << "Invalid data size for store instruction\n";
            abort();
        }
        bus.writeData(addrToWrite, rs2_, DataSizeType(1 << (func3_)));
    }

    size_t Store::takeRS1Idx() { return BitsManipulation::takeBits(inst_, 15, 19); }

    size_t Store::takeRS2Idx() { return BitsManipulation::takeBits(inst_, 20, 24); }

    AddrType Store::takeOffset()
    {
        return BitsManipulation::extendSign(
            BitsManipulation::takeBits(inst_, 7, 11)
                | (BitsManipulation::takeBits(inst_, 25, 31) << 5),
            11);
    }

    u8 Store::takeFunc3() { return BitsManipulation::takeBits(inst_, 12, 14); }
}    // namespace rvemu
