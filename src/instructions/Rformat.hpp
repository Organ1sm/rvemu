#pragma once

#include "../Cpu.hpp"
#include "InstFormat.hpp"

namespace rvemu
{
    ///
    /// 31         25 24     20 19     15 14    12 11     7 6         0
    /// +------------+---------+---------+-----------------+----------+
    /// |   funct7   |   rs2   |   rs1   | funct3 |   rd   |  opcode  | R-type
    /// +------------+---------+---------+--------+---------+---------+
    ///       7          5          5         3        5        7
    ///
    class R : public InstructionFormat
    {
      public:
        /// Reads the values of the source registers from the provided Registers object.
        void readRegister(const Registers &) override;

        /// Writes the result into the destination register in the provided Registers object.
        void writeBack(Registers &) override;

      protected:
        // Enum class representing the type of R-format operation by fusing func3 and func7 bits.
        // This is specific to R-format instructions and does not apply to all immediate
        // instructions.
        // clang-format off
        enum class Type : u16 {
            Add  = 0b0000000'000,
            Mul  = 0b0000001'000,
            Sub  = 0b0100000'000,
            Sll  = 0b0000000'001,
            Slt  = 0b0000000'010,
            Sltu = 0b0000000'011,
            Xor  = 0b0000000'100,
            Srl  = 0b0000000'101,
            Sra  = 0b0100000'101,
            Or   = 0b0000000'110,
            And  = 0b0000000'111,
        };

        // clang-format on

        /// Constructor for R-format instruction, initializing the instruction format and
        /// registers.
        R(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rs1Idx_(takeRegS1()), rs2Idx_(takeRegS2()),
            rdIdx_(takeRegD()), type(takeId())
        { }

      protected:
        size_t takeRegS1();    /// Extracts the source register 1 index from the instruction.
        size_t takeRegS2();    /// Extracts the source register 2 index from the instruction.
        size_t takeRegD();     /// Extracts the destination register index from the instruction.
        Type takeId();         /// Determines the R-format operation type based on the instruction
                               /// encoding.

        size_t rs1Idx_;        /// Index of source register 1.
        size_t rs2Idx_;        /// Index of source register 2.
        size_t rdIdx_;         /// Index of destination register.
        Type type;             /// Type of R-format operation.

        RegisterSizeType rs1_;    ///  Value of source register 1.
        RegisterSizeType rs2_;    ///  Value of source register 2.
        RegisterSizeType rd_;     ///  Value of destination register.
    };

    class Op : public R
    {
      public:
        Op(const InstSizeType is, const AddrType pc) : R(is, pc) { }

        /// Executes the specific R-format operation based on the decoded type.
        void execution() override;

      private:
        void add();
        void mul();
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

    class Op64 : public R
    {
      public:
        Op64(const InstSizeType is, const AddrType pc) : R(is, pc) { }

        /// Executes the specific R-format operation based on the decoded type.
        void execution() override;

      private:
        void addw();
        void subw();
        void sllw();
        void srlw();
        void sraw();
    };

    class ModeRet : public R
    {
      public:
        ModeRet(const InstSizeType is, const AddrType pc, CPU &cpu)
          : R(is, pc), func7_(takeFunc7()), cpu_(cpu)
        { }

        void readCsr(const CSRInterface &csr) override;

        void writeCsr(CSRInterface &csr) override;

        void execution() override;

        AddrType moveNextInst() override;

        enum class Func7Type : u8 {
            Sret      = 0x8,
            SFenceVMA = 0x9,
            Mret      = 0x18,
        };

      protected:
        Func7Type takeFunc7();

        Func7Type func7_;
        u16 csrAddr_;
        RegisterSizeType csrValue_;
        AddrType nextInst_;
        CPU &cpu_;

      private:
        void sret();
        void mret();
        void sfenceVMA();
    };
}    // namespace rvemu
