#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    ///
    /// 31            20 19     15 14      12 11     7 6         0
    /// +---------------+---------+----------+--------+----------+
    /// |   imm[11:0]   |   rs1   |  funct3  |   rd   |  opcode  | I-type
    /// +---------------+---------+----------+--------+----------+
    ///         12           5          3         5        7
    ///

    /// I-format instruction representation for RISC-V emulator
    class I : public InstructionFormat
    {
      public:
        // Reads the required registers for the I-format instruction
        void readRegister(const Registers &) override;

        // Writes the result back to the register after execution
        void writeBack(Registers &) override;

      protected:
        I(InstSizeType is, AddrType pc)
          : InstructionFormat(is, pc), rsIdx_(takeRs()), rdIdx_(takeRd()), offset_(takeOffset()),
            func3_(takeFunc3())
        { }

      private:
        // Extracts the source register index from the instruction
        std::size_t takeRs();

        // Extracts the destination register index from the instruction
        std::size_t takeRd();

        // Extracts the immediate offset value from the instruction
        RegisterSizeType takeOffset();

        // Extracts the function 3-bit code from the instruction
        u8 takeFunc3();

      protected:
        std::size_t rsIdx_;          // Index of source register
        std::size_t rdIdx_;          // Index of destination register
        RegisterSizeType offset_;    // Immediate offset for the instruction
        u8 func3_;                   // Function 3-bit code for the instruction

        RegisterSizeType rs_;        // Value of source register
        RegisterSizeType rd_;        // Value of destination register (to be written back)
    };

    class ImmOp : public I
    {
      private:
        // Enumeration for function 3-bit code (func3) to distinguish between different
        // instruction types. Some func3 values require additional information from the immediate
        // value's last two bits.
        enum class Func3Type : u8 {
            Addi       = 0b000,    // Add Immediate
            Slli       = 0b001,    // Shift Left Logical Immediate
            Slti       = 0b010,    // Set Less Than Immediate
            Sltiu      = 0b011,    // Set Less Than Immediate Unsigned
            Xori       = 0b100,    // Exclusive OR Immediate
            SrliOrSrai = 0b101,    // Shift Right Logical/Arithmetic Immediate
            Ori        = 0b110,    // OR Immediate
            Andi       = 0b111,    // AND Immediate
        };

      public:
        ImmOp(const InstSizeType is, const AddrType pc) : I(is, pc), func3_ {I::func3_} { }

        /**
         * Executes the instruction based on the decoded func3 value.
         * Calls the respective operation method associated with the func3.
         */
        void execution() override;

      private:
        // Private methods for handling each specific immediate operation.
        // These methods are called from the execution() method.
        void addi();     // Handles the ADDI instruction.
        void slti();     // Handles the SLTI instruction.
        void sltiu();    // Handles the SLTIU instruction.
        void xori();     // Handles the XORI instruction.
        void ori();      // Handles the ORI instruction.
        void andi();     // Handles the ANDI instruction.
        void slli();     // Handles the SLLI instruction.
        void srli();     // Handles the SRLI instruction.
        void srai();     // Handles the SRAI instruction.

        void printInstruction(const std::string &is_name, const std::string &op);

        // func3 value used to identify the instruction type within the I-format class.
        Func3Type func3_;
    };

    class Jris : public I
    {
      public:
        Jris(const InstSizeType is, const AddrType pc) : I(is, pc) { }

        void execution() override;
        AddrType moveNextInst() override;
        ~Jris() = default;

      private:
        RegisterSizeType reg_;
    };
}    // namespace rvemu
