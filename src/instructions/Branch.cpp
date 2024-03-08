#include "Branch.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"

#include <iostream>

namespace rvemu
{
    std::size_t Branch::takeRs1() { return BitsManipulation::takeBits(inst_, 15, 19); }

    std::size_t Branch::takeRs2() { return BitsManipulation::takeBits(inst_, 20, 24); }

    AddrType Branch::takeOffset()
    {
        const uint8_t last_digit = 12;
        AddrType of {0};
        of = BitsManipulation::takeBits(inst_, 8, 11) << 1
             | BitsManipulation::takeBits(inst_, 25, 30) << 5
             | BitsManipulation::takeBits(inst_, 7, 7) << 11
             | BitsManipulation::takeBits(inst_, 31, 31) << 12;
        return BitsManipulation::extendSign(of, last_digit);
    }

    Branch::Func3Type Branch::takeFunc3()
    {
        return Func3Type(BitsManipulation::takeBits(inst_, 12, 14));
    }

    void Branch::readRegister(const Registers &reg)
    {
        rs1_ = reg.read(rs1Idx_);
        rs2_ = reg.read(rs2Idx_);
    }

    void Branch::execution()
    {
        switch (func3_)
        {
            case Func3Type::Beq:  jump_ = beq(); break;
            case Func3Type::Bne:  jump_ = bne(); break;
            case Func3Type::Blt:  jump_ = blt(); break;
            case Func3Type::Bge:  jump_ = bge(); break;
            case Func3Type::Bltu: jump_ = bltu(); break;
            case Func3Type::Bgeu: jump_ = bgeu(); break;

            default: {
                std::cerr << "Error: func3 missed\n";
                abort();
            }
        }
    }

    bool Branch::beq() { return rs1_ == rs2_; }

    bool Branch::bne() { return rs1_ != rs2_; }

    bool Branch::blt() { return static_cast<i64>(rs1_) < static_cast<i64>(rs2_); }

    bool Branch::bltu() { return rs1_ < rs2_; }

    bool Branch::bge() { return static_cast<i64>(rs1_) >= static_cast<i64>(rs2_); }

    bool Branch::bgeu() { return rs1_ >= rs2_; }

    AddrType Branch::moveNextInst()
    {
        if (jump_ == true)
        {
            AddrType new_ins = currPC_ + offset_;
            std::cout << "condition is true: "
                      << "new instruction = "
                      << "0x" << std::hex << new_ins << std::dec << std::endl;
            return new_ins;
        }
        std::cout << "condition is false\n";
        return currPC_ + DataSizeType::Word;
    }
}    // namespace rvemu
