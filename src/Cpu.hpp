#pragma once

#include "Csr.hpp"
#include "Memory.hpp"
#include "RVEmu.hpp"
#include "Registers.hpp"

#include <memory>
#include <optional>
#include <string>

namespace rvemu
{
    class InstructionFormat;

    class CPU
    {
      public:
        CPU(const std::string &programPath);

        // Executes the instruction pipeline steps until the program ends.
        void steps();

        // Checks if the program has reached its end by comparing the program counter with the
        // last instruction address.
        bool checkEndProgram() const { return pc_ >= lastInstAddr_; }

        // Retrieves the current instruction pointed to by the program counter.
        u32 getCurrInst() const { return pc_; }

        // Returns a constant reference to the CPU registers.
        const Registers &getRegs() const { return registers_; }

        // Fetches the value of a register by its name.
        std::optional<u64> getRegValueByName(const std::string &name);

        // Prints the contents of the CPU registers.
        void dumpRegisters();

        // Prints the contents of the Control and Status Registers (CSRs).
        void dumpCSRs() const;

        // Prints the current value of the program counter.
        void dumpPC() const;

      private:
        // Enum to represent the various opcodes in the RISC-V instruction set.
        enum class OpcodeType : u8 {
            Lui     = 0b011'0111,    // Load upper immediate
            Auipc   = 0b001'0111,    // Add upper immediate to pc
            Jal     = 0b110'1111,    // Jump and link
            Jalr    = 0b110'0111,    // Jump and link register
            Branch  = 0b110'0011,    // Conditional branch
            Load    = 0b000'0011,    // Load from memory
            Store   = 0b010'0011,    // Store to memory
            Immop   = 0b001'0011,    // Immediate arithmetic operation
            Immop64 = 0b001'1011,    // Immediate arithmetic operation
            Op      = 0b011'0011,    // Register-register arithmetic operation
            Fence   = 0b000'1111,    // Memory fence operation
            System  = 0b111'0011     // System instructions
        };

        Registers registers_;        // CPU registers
        AddrType pc_;                // Program counter
        AddrType lastInstAddr_;      // Address of the last instruction in the program
        CSRInterface csrs_;          // Control and Status Registers interface
        SystemInterface bus_;        // System bus interface

        // Sets the last instruction address of the program.
        void setLastInstAddr(const AddrType lastInst) { lastInstAddr_ = lastInst; }

        // 5-stages pipeline methods:
        AddrType fetch();
        std::unique_ptr<InstructionFormat> decode(InstSizeType);
        void execute(const std::unique_ptr<InstructionFormat> &);
        void memoryAccess(const std::unique_ptr<InstructionFormat> &);
        void writeBack(const std::unique_ptr<InstructionFormat> &);
        InstSizeType moveNextInst(const std::unique_ptr<InstructionFormat> &);
    };
}    // namespace rvemu
