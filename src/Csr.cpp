#include "Csr.hpp"

#include "RVEmu.hpp"

#include <cassert>

namespace rvemu
{
    void CSRInterface::write(const AddrType dest, const RegisterSizeType what)
    {
        assert(checkLimit(dest));
        csrs_[dest] = what;
    }

    RegisterSizeType CSRInterface::read(AddrType where) const
    {
        assert(checkLimit(where));
        return csrs_[where];
    }
}    // namespace rvemu
