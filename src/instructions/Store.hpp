#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class Store : public InstructionFormat
    {
      public:
        Store(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rs1Idx_(takeRs1()), rs2Idx_(takeRs2()),
            func3_(takeFunc3()), offset_(takeOffset())
        { }

        /// Reads register values required for the store operation.
        /// @param regs Register file of the processor.
        void readRegister(const Registers &regs) override;

        /// Executes the store instruction.
        void execution() override;

        /// Accesses memory to store the value.
        /// @param sysInterface Interface to the system's memory.
        void accessMemory(SystemInterface &sysInterface) override;

      private:
        /// Extracts the RS1 register index from the instruction.
        std::size_t takeRs1();

        /// Extracts the RS2 register index from the instruction.
        std::size_t takeRs2();

        /// Extracts the offset from the instruction.
        AddrType takeOffset();

        /// Extracts the function 3-bit code from the instruction.
        u8 takeFunc3();

        std::size_t rs1Idx_;      /// Index of the RS1 register.
        std::size_t rs2Idx_;      /// Index of the RS2 register.
        u8 func3_;                /// Function 3-bit code.
        AddrType offset_;         /// Offset to be added to the base address.

        RegisterSizeType rs1_;    /// Value of the RS1 register.
        RegisterSizeType rs2_;    /// Value of the RS2 register.
        AddrType addrToWrite;     /// Computed address where data will be written.
    };
}    // namespace rvemu
