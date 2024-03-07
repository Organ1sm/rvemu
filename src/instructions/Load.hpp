#pragma once

#include "Iformat.hpp"

namespace rvemu
{
    class Load : public I
    {
      public:
        Load(const InstSizeType is, const AddrType pc) : I(is, pc) { }

        void execution() override;
        void accessMemory(SystemInterface &) override;

      private:
        AddrType addrToRead;
    };
}    // namespace rvemu
