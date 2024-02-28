#pragma once

#include <cstdint>
#include "RVEmu.h"
#include <array>

namespace rvemu
{
    // control-status-register
    class Csr
    {
      public:
        Csr();
        void dumpCSRs() const;
        uint64_t load(size_t addr) const;
        void store(size_t addr, uint64_t value);
        bool isMedelegated(uint64_t cause) const;
        bool isMidelegated(uint64_t cause) const;

      private:
        std::array<uint64_t, NUM_CSRS> csrs_;
    };
}    // namespace rvemu
