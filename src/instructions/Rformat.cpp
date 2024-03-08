
#include "Rformat.hpp"

#include "../BitsManipulation.hpp"
#include "../Registers.hpp"

#include <iostream>

namespace rvemu
{
    std::size_t R::takeRegS1() { return BitsManipulation::takeBits(inst_, 15, 19); }

    std::size_t R::takeRegS2() { return BitsManipulation::takeBits(inst_, 20, 24); }

    std::size_t R::takeRegD() { return BitsManipulation::takeBits(inst_, 7, 11); }

    R::Type R::takeId()
    {
        return Type(BitsManipulation::takeBits(inst_, 12, 14)
                    | (BitsManipulation::takeBits(inst_, 30, 31) << 3));
    }

    void R::readRegister(const Registers &reg)
    {
        rs1_ = reg.read(rs1Idx_);
        rs2_ = reg.read(rs2Idx_);
    }

    void R::writeBack(Registers &regs) { regs.write(rdIdx_, rd_); }

    void Op::add()
    {
        std::string op = "+";
        rd_            = arith(rs1_, op, rs2_);
    }

    void Op::sub()
    {
        std::string op = "-";
        rd_            = arith(rs1_, op, rs2_);
    }

    void Op::sll()
    {
        std::string op       = "<<";
        auto lower_five_bits = BitsManipulation::takeBits(rs2_, 0, 4);
        rd_                  = arith(rs1_, op, lower_five_bits);
    }

    void Op::slt()
    {
        std::string op = "<<";
        if (static_cast<int32_t>(rs1_) < static_cast<int32_t>(rs2_))
            rd_ = 1;
        else
            rd_ = 0;
    }

    void Op::sltu() { rd_ = rs1_ < rs2_ ? 1 : 0; }

    void Op::xorop()
    {
        std::string op = "^";
        rd_            = arith(rs1_, op, rs2_);
    }

    void Op::srl()
    {
        std::string op = ">>L";
        rd_            = arith(rs1_, op, BitsManipulation::takeBits(rs2_, 0, 4));
    }

    void Op::sra()
    {
        std::string op = ">>A";
        rd_            = arith(rs1_, op, BitsManipulation::takeBits(rs2_, 0, 4));
    }

    void Op::orop()
    {
        std::string op = "|";
        rd_            = arith(rs1_, op, rs2_);
    }

    void Op::andop()
    {
        std::string op = "&";
        rd_            = arith(rs1_, op, rs2_);
    }

    void Op::execution()
    {
        switch (type)
        {
            case Type::Add:  add(); break;
            case Type::Sub:  sub(); break;
            case Type::Sll:  sll(); break;
            case Type::Slt:  slt(); break;
            case Type::Sltu: sltu(); break;
            case Type::Xor:  xorop(); break;
            case Type::Srl:  srl(); break;
            case Type::Sra:  sra(); break;
            case Type::Or:   orop(); break;
            case Type::And:  andop(); break;

            default: {
                std::cerr << "Error: no matching in switch cases\n";
                abort();
            }
        }
    }

    void Op::printInstruction(const std::string &is_name, const std::string &op)
    {
        std::cout << is_name << " " << printRegIndex(rdIdx_) << " = " << printRegIndex(rs1Idx_)
                  << " " << op << " " << printRegIndex(rs2Idx_) << std::endl;
    }


}    // namespace rvemu
