#include "Exception.h"

std::ostream &operator<<(std::ostream &os, const RVEmuException &ex)
{
    switch (ex.type)
    {
        case RVEmuException::Type::InstructionAddrMisaligned:
            os << "Instruction address misaligned " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::InstructionAccessFault:
            os << "Instruction access fault " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::IllegalInstruction:
            os << "Illegal instruction " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::Breakpoint:
            os << "Breakpoint " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::LoadAccessMisaligned:
            os << "Load access " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::LoadAccessFault:
            os << "Load access fault " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::StoreAMOAddrMisaligned:
            os << "Store or AMO address misaligned " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::StoreAMOAccessFault:
            os << "Store or AMO access fault " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::EnvironmentCallFromUMode:
            os << "Environment call from U-mode " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::EnvironmentCallFromSMode:
            os << "Environment call from S-mode " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::EnvironmentCallFromMMode:
            os << "Environment call from M-mode " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::InstructionPageFault:
            os << "Instruction page fault " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::LoadPageFault:
            os << "Load page fault " << std::hex << "0x" << ex.value;
            break;
        case RVEmuException::Type::StoreAMOPageFault:
            os << "Store or AMO page fault " << std::hex << "0x" << ex.value;
            break;
    }
    return os;
}

uint64_t RVEmuException::getValue() const { return value; }

uint64_t RVEmuException::getCode() const
{
    switch (type)
    {
        case Type::InstructionAddrMisaligned: return 0;
        case Type::InstructionAccessFault: return 1;
        case Type::IllegalInstruction: return 2;
        case Type::Breakpoint: return 3;
        case Type::LoadAccessMisaligned: return 4;
        case Type::LoadAccessFault: return 5;
        case Type::StoreAMOAddrMisaligned: return 6;
        case Type::StoreAMOAccessFault: return 7;
        case Type::EnvironmentCallFromUMode: return 8;
        case Type::EnvironmentCallFromSMode: return 9;
        case Type::EnvironmentCallFromMMode: return 11;
        case Type::InstructionPageFault: return 12;
        case Type::LoadPageFault: return 13;
        case Type::StoreAMOPageFault: return 15;
    }
}

bool RVEmuException::isFatal() const
{
    switch (type)
    {
        case Type::InstructionAddrMisaligned:
        case Type::InstructionAccessFault:
        case Type::LoadAccessFault:
        case Type::StoreAMOAddrMisaligned:
        case Type::StoreAMOAccessFault:
        case Type::IllegalInstruction: return true;
        default: return false;
    }
}
