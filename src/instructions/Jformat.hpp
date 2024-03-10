#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    ///
    /// 31         30         21           19          12 11       7 6         0
    /// +---------+-------------+---------+--------------+----------+--------+
    /// | imm[20] |  imm[10:1]  | imm[11] |  imm[19:12]  |    rd    | opcode | J-type
    /// +---------+-------------+---------+--------------+----------+--------+
    ///     1           10          1            8           5           7
    /// +---------+-------------+---------+--------------+----------+--------+
    ///

    /**
     * Class Jal represents the J-type instruction format in RISC-V.
     */
    class Jal : public InstructionFormat
    {
      public:
        Jal(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), offset_(takeOffset()), rdIdx_(takeRd())
        { }

        ~Jal() = default;

        /**
         * Executes the Jal instruction logic.
         */
        void execution() override;

        /**
         * Writes back the result of the Jal instruction to the registers.
         *
         * @param regs Reference to the registers to write back to.
         */
        void writeBack(Registers &) override;

        /**
         * Calculates the address of the next instruction after Jal execution.
         *
         * @return Address of the next instruction.
         */
        AddrType moveNextInst() override;


      private:
        /**
         * Calculates the offset for the Jal instruction from the current instruction.
         *
         * @return Calculated offset.
         */
        AddrType takeOffset();

        /**
         * Extracts the index of the destination register from the current instruction.
         *
         * @return Index of the destination register.
         */
        std::size_t takeRd();

        AddrType offset_;      ///< Offset from the current instruction to the jump target.
        std::size_t rdIdx_;    ///< Index of the destination register.
    };
}    // namespace rvemu
