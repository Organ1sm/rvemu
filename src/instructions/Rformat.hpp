#pragma once

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
        // Enum class representing the type of R-format operation by fusing func3 and the last two
        // bits. This is specific to R-format instructions and does not apply to all immediate
        // instructions.
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
