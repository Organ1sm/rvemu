#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    ///
    /// 31         30       25 24   20 19   15 14      12 11       8         7 6        0
    /// +---------+-----------+-------+-------+----------+----------+---------+----------+
    /// | imm[12] | imm[10:5] |  rs2  |  rs1  |  funct3  | imm[4:1] | imm[11] |  opcode  | B-type
    /// +---------+-----------+-------+-------+----------+----------+---------+----------+
    ///     1           6         5       5        3          4           1         7
    ///
    class Branch : public InstructionFormat
    {
      public:
        Branch(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rs1Idx_(takeRs1()), rs2Idx_(takeRs2()),
            offset_(takeOffset()), func3_(takeFunc3()), jump_(false)
        { }

        // Reads the required registers for the branch instruction.
        // @param registers The set of registers from which to read.
        void readRegister(const Registers &) override;

        // Executes the branch instruction.
        void execution() override;

        // Calculates the address of the next instruction.
        // @return Address of the next instruction after branch resolution.
        AddrType moveNextInst() override;

      private:
        // Enumeration of the possible function codes for branch instructions.
        enum class Func3Type {
            Beq  = 0b000,    // Equal
            Bne  = 0b001,    // Not equal
            Blt  = 0b100,    // Less than
            Bge  = 0b101,    // Greater than or equal
            Bltu = 0b110,    // Less than unsigned
            Bgeu = 0b111     // Greater than or equal unsigned
        };

        // Extracts the rs1 register index from the instruction.
        // @return Index of the rs1 register.
        std::size_t takeRs1();

        // Extracts the rs2 register index from the instruction.
        // @return Index of the rs2 register.
        std::size_t takeRs2();

        // Extracts the branch offset from the instruction.
        // @return Computed branch offset.
        AddrType takeOffset();

        // Extracts the function code (func3) from the instruction.
        // @return The function code of the branch instruction.
        Func3Type takeFunc3();

        std::size_t rs1Idx_;      // Index of the rs1 register.
        std::size_t rs2Idx_;      // Index of the rs2 register.
        AddrType offset_;         // Offset for the branch instruction.
        Func3Type func3_;         // Function code (func3) of the branch instruction.

        RegisterSizeType rs1_;    // Value of the rs1 register.
        RegisterSizeType rs2_;    // Value of the rs2 register.
        bool jump_;               // Flag indicating whether to jump.

        // Evaluates the branch condition for 'equal'.
        // @return True if the condition is met; false otherwise.
        bool beq();

        // Evaluates the branch condition for 'not equal'.
        // @return True if the condition is met; false otherwise.
        bool bne();

        // Evaluates the branch condition for 'less than'.
        // @return True if the condition is met; false otherwise.
        bool blt();

        // Evaluates the branch condition for 'greater than or equal'.
        // @return True if the condition is met; false otherwise.
        bool bge();

        // Evaluates the branch condition for 'less than unsigned'.
        // @return True if the condition is met; false otherwise.
        bool bltu();

        // Evaluates the branch condition for 'greater than or equal unsigned'.
        // @return True if the condition is met; false otherwise.
        bool bgeu();
    };
}    // namespace rvemu
