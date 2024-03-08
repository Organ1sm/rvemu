#include "Iformat.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"
#include "InstFormat.hpp"

#include <cstdint>
#include <iostream>

namespace rvemu
{
    std::size_t I::takeRegS() { return BitsManipulation::takeBits(inst_, 15, 19); }

    std::size_t I::takeRegD() { return BitsManipulation::takeBits(inst_, 7, 11); }

    RegisterSizeType I::takeOffset()
    {
        auto offset = BitsManipulation::takeBits(inst_, 20, 31);
        return BitsManipulation::extendSign(offset, 11);
    }

    u8 I::takeFunc3() { return BitsManipulation::takeBits(inst_, 12, 14); }

    void I::readRegister(const Registers &regs) { rs_ = regs.read(rsIdx_); }

    void I::writeBack(Registers &regs) { regs.write(rdIdx_, rd_); }

    void ImmOp::addi()
    {
        std::string op = "+";
        rd_            = arith(rs_, op, offset_);
    }

    void ImmOp::slti()
    {
        if (static_cast<int32_t>(rs_)
            < static_cast<int32_t>(BitsManipulation::extendSign(offset_, 11)))
        {
            rd_ = 1;
        }
        else
        {
            rd_ = 0;
        }
    }

    void ImmOp::sltiu() { rd_ = rs_ < offset_ ? 1 : 0; }

    void ImmOp::xori()
    {
        std::string op = "^";
        rd_            = arith(rs_, op, offset_);
    }

    void ImmOp::ori()
    {
        std::string op = "|";
        rd_            = arith(rs_, op, offset_);
    }

    void ImmOp::andi()
    {
        std::string op = "&";
        rd_            = arith(rs_, op, offset_);
    }

    void ImmOp::slli()
    {
        std::string op         = "<<";
        RegisterSizeType shamt = BitsManipulation::takeBits(inst_, 20, 24);
        rd_                    = arith(rs_, op, shamt);
    }

    void ImmOp::srli()
    {
        std::string op         = ">>L";
        RegisterSizeType shamt = BitsManipulation::takeBits(inst_, 20, 24);
        rd_                    = arith(rs_, op, shamt);
    }

    void ImmOp::srai()
    {
        std::string op         = ">>A";
        RegisterSizeType shamt = BitsManipulation::takeBits(inst_, 20, 24);
        rd_                    = arith(rs_, op, shamt);
    }

    void ImmOp::execution()
    {
        switch (func3_)
        {
            case Func3Type::Addi:  addi(); break;
            case Func3Type::Slti:  slti(); break;
            case Func3Type::Sltiu: sltiu(); break;
            case Func3Type::Xori:  xori(); break;
            case Func3Type::Ori:   ori(); break;
            case Func3Type::Andi:  andi(); break;
            case Func3Type::Slli:  slli(); break;

            case Func3Type::SrliOrSrai: {
                if (BitsManipulation::takeBits(inst_, 30, 31) == 0)
                    srli();
                if (BitsManipulation::takeBits(inst_, 30, 31) == 1)
                    srai();
                break;
            }

            default: {
                std::cerr << "Error: input no matching cases\n";
                abort();
            }
        }
    }

    void ImmOp::printInstruction(const std::string &is_name, const std::string &op) { }

    // Jris
    void Jris::execution() { rd_ = currPC_ + DataSizeType::Word; }

    // NOTE: the least significant bit is not set to zero,
    // as happens with branch or jump, because Jris instructions
    // belongs to the I-format
    AddrType Jris::moveNextInst()
    {
        AddrType next_instruction = (rs_ + offset_) & (~1);    // set last digit to 0
        return next_instruction;
    }
}    // namespace rvemu
