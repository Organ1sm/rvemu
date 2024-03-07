#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class Store : public InstructionFormat
    {
      public:
        Store(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rs1Idx_(takeRS1Idx()), rs2Idx_(takeRS2Idx()),
            func3_(takeFunc3()), offset_(takeOffset())
        { }

        void readRegister(const Registers &) override;
        void execution() override;
        void accessMemory(SystemInterface &) override;

      private:
        std::size_t takeRS1Idx();
        std::size_t takeRS2Idx();
        AddrType takeOffset();
        u8 takeFunc3();

        std::size_t rs1Idx_;
        std::size_t rs2Idx_;
        u8 func3_;
        AddrType offset_;

        RegisterSizeType rs1_;
        RegisterSizeType rs2_;
        AddrType addrToWrite;
    };
}    // namespace rvemu
