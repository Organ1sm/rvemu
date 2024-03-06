#pragma once

#include "InstFormat.hpp"

#include <cstdint>

namespace rvemu
{
    class R : public InstructionFormat
    {
      public:
        void readRegister(const Registers &) override;
        void writeBack(Registers &) override;

      protected:
        enum class I : u8 {

        };
    };

}    // namespace rvemu
