
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
        m_rs1 = reg.read(m_index_rs1);
        m_rs2 = reg.read(m_index_rs2);
    }

    void R::writeBack(Registers &regs) { regs.write(m_index_rd, m_rd); }

    void Op::add()
    {
        std::string op = "+";
        m_rd           = arith(m_rs1, op, m_rs2);
    }

    void Op::sub()
    {
        std::string op = "-";
        m_rd           = arith(m_rs1, op, m_rs2);
    }

    void Op::sll()
    {
        std::string op       = "<<";
        auto lower_five_bits = BitsManipulation::takeBits(m_rs2, 0, 4);
        m_rd                 = arith(m_rs1, op, lower_five_bits);
    }

    void Op::slt()
    {
        std::string op = "<<";
        if (static_cast<int32_t>(m_rs1) < static_cast<int32_t>(m_rs2))
            m_rd = 1;
        else
            m_rd = 0;
    }

    void Op::sltu() { m_rd = m_rs1 < m_rs2 ? 1 : 0; }

    void Op::xorop()
    {
        std::string op = "^";
        m_rd           = arith(m_rs1, op, m_rs2);
    }

    void Op::srl()
    {
        std::string op = ">>L";
        m_rd           = arith(m_rs1, op, BitsManipulation::takeBits(m_rs2, 0, 4));
    }

    void Op::sra()
    {
        std::string op = ">>A";
        m_rd           = arith(m_rs1, op, BitsManipulation::takeBits(m_rs2, 0, 4));
    }

    void Op::orop()
    {
        std::string op = "|";
        m_rd           = arith(m_rs1, op, m_rs2);
    }

    void Op::andop()
    {
        std::string op = "&";
        m_rd           = arith(m_rs1, op, m_rs2);
    }

    void Op::execution()
    {
        switch (m_id)
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
        std::cout << is_name << " " << printRegIndex(m_index_rd) << " = "
                  << printRegIndex(m_index_rs1) << " " << op << " " << printRegIndex(m_index_rs2)
                  << std::endl;
    }


}    // namespace rvemu
