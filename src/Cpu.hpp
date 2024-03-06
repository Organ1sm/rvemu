#pragma once

#include "Csr.hpp"
#include "Memory.hpp"
#include "RVEmu.hpp"
#include "Registers.hpp"

#include <memory>
#include <string>

namespace rvemu
{
    class InstructionFormat;

    class CPU
    {
      public:
        CPU(const std::string &);
        void steps();

        bool checkEndProgram() { return pc_ >= lastInstAddr_; }

        u32 getCurrentInstruction() { return pc_; }

        void dumpRegisters();
        void dumpPC() const;

      private:
        enum class OpcodeType : u8 {
            Lui    = 0b011'0111,    // load upper immediate lui
            Auipc  = 0b001'0111,
            Jal    = 0b110'1111,
            Jalr   = 0b110'0111,
            Branch = 0b110'0011,
            Load   = 0b000'0011,
            Store  = 0b010'0011,
            Immop  = 0b001'0011,
            Op     = 0b011'0011,
            Fence  = 0b000'1111,
            System = 0b111'0011,
        };

      private:
        Registers registers_;
        AddrType pc_;
        AddrType lastInstAddr_;
        CSRInterface csrs_;
        SystemInterface bus_;

      private:
        void setLastInstAddr(const AddrType lastInst) { lastInstAddr_ = lastInst; }

        // 5-stages pipeline
        AddrType fetch();
        std::unique_ptr<InstructionFormat> decode(InstSizeType);
        void execute(const std::unique_ptr<InstructionFormat> &);
        void memoryAccess(const std::unique_ptr<InstructionFormat> &);
        void writeBack(const std::unique_ptr<InstructionFormat> &);
        InstSizeType moveNextInst(const std::unique_ptr<InstructionFormat> &);
    };
}    // namespace rvemu
