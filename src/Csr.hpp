#pragma once

#include "RVEmu.hpp"

namespace rvemu
{
    class CSRInterface
    {
      public:
        CSRInterface() : csrs_ {0} { }

        void write(AddrType, RegisterSizeType);
        RegisterSizeType read(AddrType) const;

      private:
        bool checkLimit(AddrType a) const { return a < NUM_CSRS; }

      private:
        std::array<CSRRegisterSizeType, NUM_CSRS> csrs_;
    };

}    // namespace rvemu
