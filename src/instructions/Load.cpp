#include "Load.hpp"

#include "../BitsManipulation.hpp"
#include "../Memory.hpp"

#include <cassert>
#include <iostream>

namespace rvemu
{
    void Load::execution() { addrToRead = rs_ + offset_; }

    void Load::accessMemory(SystemInterface &bus)
    {
        assert(func3_ <= 6);
        DataSizeType sz;
        if (func3_ == 0 || func3_ == 4)         // lb or lbu
            sz = Byte;
        else if (func3_ == 1 || func3_ == 5)    // lh or lhu
            sz = HalfWord;
        else if (func3_ == 2 || func3_ == 6)    // lw or lwu
            sz = Word;
        else
        {
            std::cerr << "Invalid func3 in load instruction\n";
            abort();
        }
        rd_ = bus.readData(addrToRead, sz);

        if (func3_ <= 2)
            rd_ = BitsManipulation::extendSign(rd_, (1 << (func3_ + 3)) - 1);
    }
}    // namespace rvemu
