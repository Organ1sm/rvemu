#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class System : public InstructionFormat
    {
      public:
        System(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rdIdx_(takeRd()), qty_(takeQty()), func3_(takeFunc3()),
            func12_(takeFunc12())
        { }

        /// Enumeration for the different types of functions in a System instruction.
        enum class Func3Type : u8 {
            Ecall  = 0b000,    // System Call
            Ebreak = 0b000,    // Breakpoint
            Csrrw  = 0b001,    // Read and Write CSR
            Csrrs  = 0b010,    // Read and Set bits in CSR
            Csrrc  = 0b011,    // Read and clear bits in CSR
            Csrrwi = 0b101,
            Csrrsi = 0b110,
            Csrrci = 0b111
        };

      private:
        std::size_t takeRd();    /// Extracts the destination register index from the instruction.
        u8 takeQty();            /// Gets the quantity from the instruction.
        Func3Type takeFunc3();    /// Extracts the 3-bit function field from the instruction.
        u16 takeFunc12();         /// Extracts the 12-bit function field from the instruction.

      protected:
        std::size_t rdIdx_;       // Index of the destination register.
        u8 qty_;             // Quantity to distinguish from immediate and normal CSR operations.
        Func3Type func3_;    // Function 3-bit code.
        u16 func12_;         // Function 12-bit code.
    };

    class CSR : public System
    {
      public:
        CSR(const InstSizeType is, const AddrType pc)
          : System(is, pc), writeToReg_ {true}, writeToCSRS_ {true}
        { }

        /// Reads the values of the registers required for the CSR instruction.
        /// @param regs Reference to the registers to read from.
        void readRegister(const Registers &regs) override;

        /// Reads the value of the CSR specified by the instruction.
        /// @param csr Reference to the CSR interface to read from.
        void readCsr(const CSRInterface &csr) override;

        /// Executes the CSR instruction.
        void execution() override;

        /// Writes the result of the CSR operation to the CSR registers.
        /// @param csr Reference to the CSR interface to write to.
        void writeCsr(CSRInterface &csr) override;

        /// Writes the result of the CSR operation back to the registers.
        /// @param regs Reference to the registers to write back to.
        void writeBack(Registers &regs) override;

      private:
        /// Generates the result for the CSR operation based on the instruction encoding.
        /// @return The result of the CSR operation.
        CSRRegisterSizeType makeCSRResult();

        /// Determines if the CSR operation is a write operation.
        /// @return True if it is a write operation, otherwise false.
        bool isWriteOp();

      private:
        RegisterSizeType secondOperand_;    // Operand from the immediate or register.
        CSRRegisterSizeType csrRS_;    // Source CSR register value (alias t in RISC-V manual).
        CSRRegisterSizeType csrRD_;    // Destination CSR register value.
        RegisterSizeType rd_;          // Destination general-purpose register.
        bool writeToReg_;              // Flag to indicate if write-back to register is needed.
        bool writeToCSRS_;             // Flag to indicate if write to CSR register is needed.
    };
}    // namespace rvemu
