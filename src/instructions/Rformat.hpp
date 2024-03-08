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
          : InstructionFormat(is, pc), rs1Idx_(takeRegS1()), rs2Idx_(takeRegS2()),
            rdIdx_(takeRegD()), type(takeId())
        { }

      protected:
        size_t takeRegS1();
        size_t takeRegS2();
        size_t takeRegD();
        Type takeId();

        size_t rs1Idx_;
        size_t rs2Idx_;
        size_t rdIdx_;
        Type type;

        RegisterSizeType rs1_;
        RegisterSizeType rs2_;
        RegisterSizeType rd_;
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
