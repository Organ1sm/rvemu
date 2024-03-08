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

        enum class Func3Type : u8 {
            Ecall  = 0b000,
            Ebreak = 0b000,
            Csrrw  = 0b001,
            Csrrs  = 0b010,
            Srrc   = 0b011,
            Csrrwi = 0b101,
            Srrsi  = 0b110,
            Csrrci = 0b111
        };

      private:
        std::size_t takeRd();
        u8 takeQty();
        Func3Type takeFunc3();
        u16 takeFunc12();

      protected:
        std::size_t rdIdx_;
        u8 qty_;    // to distinguish from immediate and normal csrs opearation
        Func3Type func3_;
        u16 func12_;
    };

    class CSR : public System
    {
      public:
        CSR(const InstSizeType is, const AddrType pc)
          : System(is, pc), writeToReg_ {true}, writeToCSRS_ {true}
        { }

        void readRegister(const Registers &) override;
        void readCsr(const CSRInterface &) override;
        void execution() override;
        void writeCsr(CSRInterface &) override;
        void writeBack(Registers &) override;

      private:
        CSRRegisterSizeType makeCSRResult();
        bool isWriteOp();

      private:
        RegisterSizeType secondOperand_;
        CSRRegisterSizeType csrRS_;    // alias t in riscv manual
        CSRRegisterSizeType csrRD_;
        RegisterSizeType rd_;
        bool writeToReg_;
        bool writeToCSRS_;
    };
}    // namespace rvemu
