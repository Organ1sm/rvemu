#pragma once

#include "InstFormat.hpp"

namespace rvemu
{

    ///
    /// 31                                 12 11     7 6         0
    /// +------------------------------------+--------+----------+
    /// |              imm[31:12]            |   rd   |  opcode  | U-type
    /// +------------------------------------+--------+----------+
    ///                   20                     5         7
    ///
    class U : public InstructionFormat
    {
      public:
        U(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), rdIdx_(takeRd()), offset_(takeOffset())
        { }

        void writeBack(Registers &) override;

      protected:
        RegisterSizeType takeOffset();
        size_t takeRd();

        size_t rdIdx_;
        RegisterSizeType offset_;
        RegisterSizeType rd_;
    };

    /// AUIPC - Add Upper Immediate to pc
    class Auipc : public U
    {
      public:
        Auipc(const InstSizeType is, const AddrType pc) : U(is, pc) { }

        void execution() override;
    };

    /// LUI - Load Upper Immediate
    class Lui : public U
    {
      public:
        Lui(const InstSizeType is, const AddrType pc) : U(is, pc) { }

        void execution() override;
    };
}    // namespace rvemu
