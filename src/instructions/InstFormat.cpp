#include "InstFormat.hpp"

#include "../Registers.hpp"

#include <iostream>

// Default functions that each instruction format will override
namespace rvemu
{
    void InstructionFormat::readRegister(const Registers &) { }

    void InstructionFormat::readCsr(const CSRInterface &) { }

    void InstructionFormat::accessMemory(SystemInterface &) { }

    void InstructionFormat::writeCsr(CSRInterface &) { }

    void InstructionFormat::writeBack(Registers &) { }

    AddrType InstructionFormat::moveNextInst() { return currPC_ + DataSizeType::Word; }

    std::string InstructionFormat::printRegIndex(const std::size_t reg_ind)
    {
        return "x" + std::to_string(reg_ind) + "(" + Registers::getABIName(reg_ind) + ")";
    }

    auto arith(RegisterSizeType lhs, const std::string &op, RegisterSizeType rhs)
        -> RegisterSizeType
    {
        if (op == "+")
        {
            return lhs + rhs;
        }
        else if (op == "-")
        {
            return lhs - rhs;
        }
        else if (op == "<<")
        {
            return lhs << rhs;
        }
        else if (op == ">>A")
        {
            return static_cast<int32_t>(lhs) >> rhs;
        }
        else if (op == ">>L")
        {
            return lhs >> rhs;
        }
        else if (op == "^")
        {
            return lhs ^ rhs;
        }
        else if (op == "|")
        {
            return lhs | rhs;
        }
        else if (op == "&")
        {
            return lhs & rhs;
        }
        else
        {
            std::cerr << "Error: operator " << op << " doesn't match\n";
            abort();
        }
    }
}    // namespace rvemu
