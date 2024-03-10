#pragma once

#include "../RVEmu.hpp"

#include <string>

namespace rvemu
{
    class Registers;
    class CSRInterface;
    class SystemInterface;

    RegisterSizeType arith(RegisterSizeType lhs, const std::string &op, RegisterSizeType rhs);

    class InstructionFormat
    {
      public:
        InstructionFormat(InstSizeType is, AddrType pc) : inst_(is), currPC_(pc) { }

        /// Read register values and populate internal fields.
        virtual void readRegister(const Registers &);

        /// Read CSR values and populate internal fields.
        virtual void readCsr(const CSRInterface &);

        /// Execute the instruction logic.
        virtual void execution() = 0;

        /// Perform any required memory access.
        virtual void accessMemory(SystemInterface &);

        /// Write any changes to the control status registers.
        virtual void writeCsr(CSRInterface &);

        /// Write the result back to the registers.
        virtual void writeBack(Registers &);

        /// Compute the address of the next instruction.
        virtual AddrType moveNextInst();

        virtual ~InstructionFormat() = default;

      protected:
        /// Helper function to print register indices.
        std::string printRegIndex(std::size_t);

      protected:
        const InstSizeType inst_;    /// The instruction.
        const AddrType currPC_;      /// The current program counter.
    };
}    // namespace rvemu
