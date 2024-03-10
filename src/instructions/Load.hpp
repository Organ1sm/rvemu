#pragma once

#include "Iformat.hpp"

namespace rvemu
{
    /// Load also I format insturction.
    class Load : public I
    {
      public:
        Load(const InstSizeType is, const AddrType pc) : I(is, pc) { }

        void execution() override;

        /// Accesses memory based on the Load instruction.
        ///
        /// @param systemInterface A reference to the system interface to interact with memory.
        void accessMemory(SystemInterface &) override;

      private:
        AddrType
            addrToRead;    /// Holds the calculated address to read from during memory access.
    };
}    // namespace rvemu
