#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class R : public InstructionFormat
    {
      public:
        void readRegister(const Registers &) override;
        void writeBack(Registers &) override;

      protected:
        // fusion of func3 and the last two bits. Not all immediate
        // instructions have this properties so for I-format has not
        // been defined.
        enum class Type : u8 {
            Add  = 0b0'0000,
            Sub  = 0b0'1000,
            Sll  = 0b0'0001,
            Slt  = 0b0'0010,
            Sltu = 0b0'0011,
            Xor  = 0b0'0100,
            Srl  = 0b0'0101,
            Sra  = 0b0'1101,
            Or   = 0b0'0110,
            And  = 0b0'0111,
        };

        R(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), m_index_rs1(takeRegS1()), m_index_rs2(takeRegS2()),
            m_index_rd(takeRegD()), m_id(takeId())
        { }

      protected:
        size_t takeRegS1();
        size_t takeRegS2();
        size_t takeRegD();
        Type takeId();

        size_t m_index_rs1;
        size_t m_index_rs2;
        size_t m_index_rd;
        Type m_id;

        RegisterSizeType m_rs1;
        RegisterSizeType m_rs2;
        RegisterSizeType m_rd;
    };

    class Op : public R
    {
      public:
        Op(const InstSizeType is, const AddrType pc) : R(is, pc) { }

        void execution() override;

      private:
        void add();
        void sub();
        void sll();
        void slt();
        void sltu();
        void xorop();
        void srl();
        void sra();
        void orop();
        void andop();
        void printInstruction(const std::string &instName, const std::string &op);
    };
}    // namespace rvemu
