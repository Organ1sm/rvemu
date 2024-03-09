#pragma once

#include "RVEmu.hpp"

#include <string>
#include <unordered_map>

namespace rvemu
{
    class CSRInterface
    {
      public:
        CSRInterface() : csrs_ {0} { }

        void write(AddrType, RegisterSizeType);
        RegisterSizeType read(AddrType) const;

        void dumpCSRs() const;

      public:
        static const std::unordered_map<std::string, std::size_t> csrAddrs_;

      private:
        bool checkLimit(AddrType a) const { return a < NUM_CSRS; }

      private:
        std::array<CSRRegisterSizeType, NUM_CSRS> csrs_;
    };

}    // namespace rvemu
