#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class Jal : public InstructionFormat
    {
      public:
        Jal(const InstSizeType is, const AddrType pc)
          : InstructionFormat(is, pc), offset_(takeOffset()), rdIdx_(takeIndexRd())
        { }

        void execution() override;
        void writeBack(Registers &) override;
        AddrType moveNextInst() override;
        ~Jal() = default;

      private:
        AddrType takeOffset();
        std::size_t takeIndexRd();

        AddrType offset_;
        std::size_t rdIdx_;
    };
}    // namespace rvemu
