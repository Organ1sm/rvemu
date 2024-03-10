#include "Branch.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"

#include <iostream>

namespace rvemu
{
    std::size_t Branch::takeRs1() { return BitsManipulation::takeBits(inst_, 15, 19); }

    std::size_t Branch::takeRs2() { return BitsManipulation::takeBits(inst_, 20, 24); }

    /**
     * @brief Extract the offset for a branch instruction.
     *
     * This method constructs the branch offset from various bits of the instruction.
     * It uses the BitsManipulation utility to extract the relevant bits and then
     * assembles them into the offset. The offset is sign-extended to match the
     * correct value for branching.
     *
     * @return The sign-extended offset for the branch instruction.
     */
    AddrType Branch::takeOffset()
    {
        const u8 lastDigit = 12;    // The bit position for sign extension.

        // Initialize the offset variable.
        AddrType of {0};

        // Construct the offset from the instruction bits.
        of = BitsManipulation::takeBits(inst_, 8, 11) << 1    // Bits 8-11 are shifted left by 1.
             | BitsManipulation::takeBits(inst_, 25, 30)
                   << 5                                       // Bits 25-30 are shifted left by 5.
             | BitsManipulation::takeBits(inst_, 7, 7) << 11    // Bit 7 is shifted left by 11.
             | BitsManipulation::takeBits(inst_, 31, 31)
                   << 12;                                       // Bit 31 is shifted left by 12.

        // Return the sign-extended offset.
        return BitsManipulation::extendSign(of, lastDigit);
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
