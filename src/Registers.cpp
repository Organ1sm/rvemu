#include "Registers.hpp"
#include <cassert>


namespace rvemu
{
    std::string Registers::getABIName(std::size_t index)
    {
        assert(index < RegistersNumber);
        return RVABI[index];
    }

}    // namespace rvemu
