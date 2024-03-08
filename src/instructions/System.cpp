#include "System.hpp"

#include "../BitsManipulation.hpp"
#include "../Csr.hpp"
#include "../Registers.hpp"

#include <iostream>

namespace rvemu
{
    std::size_t System::takeRd() { return BitsManipulation::takeBits(inst_, 7, 11); }

    uint8_t System::takeQty() { return BitsManipulation::takeBits(inst_, 15, 19); }

    System::Func3Type System::takeFunc3()
    {
        return Func3Type(BitsManipulation::takeBits(inst_, 12, 14));
    }

    uint16_t System::takeFunc12() { return BitsManipulation::takeBits(inst_, 20, 31); }

    bool CSR::isWriteOp()
    {
        return func3_ == System::Func3Type::Csrrw || func3_ == System::Func3Type::Csrrwi;
    }

    void CSR::readRegister(const Registers &reg)
    {
        secondOperand_ = BitsManipulation::takeBits(inst_, 15, 19);
        if (secondOperand_ == 0 && isWriteOp() == false)
        {
            writeToCSRS_ = false;
        }
        if (isWriteOp() == true && rdIdx_ == 0)
        {
            writeToReg_ = false;
        }
        if (static_cast<uint8_t>(func3_) >= 5)
        {
            secondOperand_ = reg.read(secondOperand_);
        }
    }

    void CSR::readCsr(const CSRInterface &csrs) { csrRS_ = csrs.read(func12_); }

    void CSR::execution()
    {
        csrRD_ = makeCSRResult();
        rd_    = csrRS_;
    }

    void CSR::writeCsr(CSRInterface &csrs)
    {
        if (writeToCSRS_ == true)
            csrs.write(func12_, csrRD_);
    }

    void CSR::writeBack(Registers &reg)
    {
        if (writeToReg_ == true)
            reg.write(rdIdx_, rd_);
    }

    CSRRegisterSizeType CSR::makeCSRResult()
    {
        if (func3_ == System::Func3Type::Csrrw || func3_ == System::Func3Type::Csrrwi)
        {
            return secondOperand_;
        }
        else if (func3_ == System::Func3Type::Csrrs || func3_ == System::Func3Type::Srrsi)
        {
            return arith(csrRS_, "|", secondOperand_);
        }
        else if (func3_ == System::Func3Type::Srrc || func3_ == System::Func3Type::Csrrci)
        {
            return arith(csrRS_, "&", !secondOperand_);
        }
        else
        {
            std::cerr << "Error: func3 doesn't match for csrs operations\n";
            abort();
        }
    }

}    // namespace rvemu
