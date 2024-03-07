#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class Branch : public InstructionFormat
    {
      public:
        Branch(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rs1Idx_(takeRs1()), rs2Idx_(takeRs2()),
            offset_(takeOffset()), func3_(takeFunc3()), jump_(false)
        { }

        void readRegister(const Registers &) override;

        void execution() override;
        AddrType moveNextInst() override;

      private:
        enum class Func3Type {
            Beq  = 0b000,
            Bne  = 0b001,
            Blt  = 0b100,
            Bge  = 0b101,
            Bltu = 0b110,
            Bgeu = 0b111
        };

        std::size_t takeRs1();
        std::size_t takeRs2();
        AddrType takeOffset();
        Func3Type takeFunc3();

        std::size_t rs1Idx_;
        std::size_t rs2Idx_;
        AddrType offset_;
        Func3Type func3_;

        RegisterSizeType rs1_;
        RegisterSizeType rs2_;
        bool jump_;

        bool beq();
        bool bne();
        bool blt();
        bool bge();
        bool bltu();
        bool bgeu();
    };



}    // namespace rvemu
