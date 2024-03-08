#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
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

    class Auipc : public U
    {
      public:
        Auipc(const InstSizeType is, const AddrType pc) : U(is, pc) { }

        void execution() override;
    };

    class Lui : public U
    {
      public:
        Lui(const InstSizeType is, const AddrType pc) : U(is, pc) { }

        void execution() override;
    };
}    // namespace rvemu
