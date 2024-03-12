
#include "Rformat.hpp"

#include "../BitsManipulation.hpp"
#include "../Cpu.hpp"
#include "../Csr.hpp"
#include "../Registers.hpp"

#include <cstdint>
#include <iostream>

namespace rvemu
{
    std::size_t R::takeRegS1() { return BitsManipulation::takeBits(inst_, 15, 19); }

    std::size_t R::takeRegS2() { return BitsManipulation::takeBits(inst_, 20, 24); }

    std::size_t R::takeRegD() { return BitsManipulation::takeBits(inst_, 7, 11); }

    R::Type R::takeId()
    {
        return Type(BitsManipulation::takeBits(inst_, 12, 14)
                    | (BitsManipulation::takeBits(inst_, 25, 31) << 3));
    }

    void R::readRegister(const Registers &reg)
    {
        rs1_ = reg.read(rs1Idx_);
        rs2_ = reg.read(rs2Idx_);
    }

    void R::writeBack(Registers &regs) { regs.write(rdIdx_, rd_); }

    void Op::add() { rd_ = rs1_ + rs2_; }

    void Op::mul() { rd_ = rs1_ * rs2_; }

    void Op::sub() { rd_ = rs1_ - rs2_; }

    void Op::sll()
    {
        auto shamt = BitsManipulation::takeBits(rs2_, 0, 5);
        rd_        = rs1_ << shamt;
    }

    void Op::slt()
    {
        if (static_cast<int32_t>(rs1_) < static_cast<int32_t>(rs2_))
            rd_ = 1;
        else
            rd_ = 0;
    }

    void Op::sltu() { rd_ = rs1_ < rs2_ ? 1 : 0; }

    void Op::xorop() { rd_ = rs1_ ^ rs2_; }

    void Op::srl() { rd_ = rs1_ >> BitsManipulation::takeBits(rs2_, 0, 5); }

    void Op::sra() { rd_ = static_cast<int32_t>(rs1_) >> BitsManipulation::takeBits(rs2_, 0, 5); }

    void Op::orop() { rd_ = rs1_ | rs2_; }

    void Op::andop() { rd_ = rs1_ & rs2_; }

    void Op::execution()
    {
        switch (type)
        {
            case Type::Add:  add(); break;
            case Type::Mul:  mul(); break;
            case Type::Sub:  sub(); break;
            case Type::Sll:  sll(); break;
            case Type::Slt:  slt(); break;
            case Type::Sltu: sltu(); break;
            case Type::Xor:  xorop(); break;
            case Type::Srl:  srl(); break;
            case Type::Sra:  sra(); break;
            case Type::Or:   orop(); break;
            case Type::And:  andop(); break;

            default: {
                std::cerr << "Error: no matching in switch cases\n";
                abort();
            }
        }
    }

    void Op::printInstruction(const std::string &is_name, const std::string &op)
    {
        std::cout << is_name << " " << printRegIndex(rdIdx_) << " = " << printRegIndex(rs1Idx_)
                  << " " << op << " " << printRegIndex(rs2Idx_) << std::endl;
    }

    void Op64::addw() { rd_ = BitsManipulation::extendSign(rs1_ + rs2_, 31); }

    void Op64::subw() { rd_ = BitsManipulation::extendSign(rs1_ - rs2_, 31); }

    void Op64::sllw()
    {
        auto shamt = BitsManipulation::takeBits(rs2_, 0, 4);
        rd_        = BitsManipulation::extendSign((rs1_ & 0xFFFF'FFFF) << shamt, 31);
    }

    void Op64::srlw()
    {
        auto shamt = BitsManipulation::takeBits(rs2_, 0, 4);
        rd_        = BitsManipulation::extendSign((rs1_ & 0xFFFF'FFFF) >> shamt, 31);
    }

    void Op64::sraw()
    {
        int shamt = BitsManipulation::takeBits(rs2_, 0, 4);
        rd_       = BitsManipulation::extendSign(static_cast<int32_t>(rs1_) >> shamt, 31);
    }

    void Op64::execution()
    {
        switch (type)
        {
            case Type::Add: addw(); break;
            case Type::Sub: subw(); break;
            case Type::Sll: sllw(); break;
            case Type::Srl: srlw(); break;
            case Type::Sra: sraw(); break;

            default: {
                std::cerr << "Error: no matching in switch cases\n";
                abort();
            }
        }
    }

    ModeRet::Func7Type ModeRet::takeFunc7()
    {
        auto func7 = Func7Type(BitsManipulation::takeBits(inst_, 25, 31));
        switch (func7)
        {
            case Func7Type::Sret: this->csrAddr_ = SSTATUS; break;
            case Func7Type::Mret: this->csrAddr_ = MSTATUS; break;

            default: break;
        }
        return func7;
    }

    void ModeRet::sret()
    {
        u64 sstatus = csrValue_;
        Mode mode   = (sstatus & MASK_SPP) >> 8;
        cpu_.setMode(mode);
        u64 spie = (sstatus & MASK_SPIE) >> 5;
        sstatus  = (sstatus & ~MASK_SIE) | (spie << 1);
        sstatus |= MASK_SPIE;
        sstatus &= ~MASK_SPP;
        csrValue_ = sstatus;
    }

    void ModeRet::mret()
    {
        u64 mstauts = csrValue_;
        Mode mode   = (mstauts & MASK_SPP) >> 11;
        cpu_.setMode(mode);
        u64 mpie = (mstauts & MASK_MPIE) >> 5;
        mstauts  = (mstauts & ~MASK_SIE) | (mpie << 3);
        mstauts |= MASK_MPIE;
        mstauts &= ~MASK_MPP;
        if ((mstauts & MASK_MPP) != Machine)
            mstauts &= ~MASK_MPRV;

        csrValue_ = mstauts;
    }

    void ModeRet::sfenceVMA() { nextInst_ = currPC_ + DataSizeType::Word; }

    void ModeRet::execution()
    {
        switch (func7_)
        {
            case Func7Type::Sret:      sret(); break;
            case Func7Type::Mret:      mret(); break;
            case Func7Type::SFenceVMA: sfenceVMA(); break;

            default: {
                std::cerr << "Error: no matching in switch cases\n";
                abort();
            }
        }
    }

    void ModeRet::readCsr(const CSRInterface &csrs) { csrValue_ = csrs.read(csrAddr_); }

    void ModeRet::writeCsr(CSRInterface &csrs)
    {
        if (func7_ == Func7Type::Sret || func7_ == Func7Type::Mret)
            csrs.write(csrAddr_, csrValue_);

        switch (func7_)
        {
            case Func7Type::Sret: nextInst_ = csrs.read(SEPC) & ~0b11; break;
            case Func7Type::Mret: nextInst_ = csrs.read(MEPC) & ~0b11; break;

            default: break;
        }
    }

    AddrType ModeRet::moveNextInst() { return nextInst_; }
}    // namespace rvemu
