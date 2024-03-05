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

        // it prepares the information that will use in the next stages;
        // for instance rd, rs etc
        virtual void readRegister(const Registers &);
        virtual void readCsr(const CSRInterface &);
        virtual void execution() = 0;
        virtual void accessMemory(SystemInterface &);
        virtual void writeCsr(CSRInterface &);
        virtual void writeBack(Registers &);
        virtual AddrType moveNextInst();
        virtual ~InstructionFormat() = default;

      protected:
        std::string printRegIndex(std::size_t);

      protected:
        const InstSizeType inst_;
        const AddrType currPC_;
    };
}    // namespace rvemu
