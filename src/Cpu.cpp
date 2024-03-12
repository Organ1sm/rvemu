#include "Cpu.hpp"

#include "BitsManipulation.hpp"
#include "Csr.hpp"
#include "RVEmu.hpp"
#include "instructions/Branch.hpp"
#include "instructions/Fence.hpp"
#include "instructions/Iformat.hpp"
#include "instructions/InstFormat.hpp"
#include "instructions/Jformat.hpp"
#include "instructions/Load.hpp"
#include "instructions/Rformat.hpp"
#include "instructions/Store.hpp"
#include "instructions/System.hpp"
#include "instructions/Uformat.hpp"

#include <algorithm>
#include <fmt/core.h>
#include <iterator>
#include <memory>

namespace rvemu
{
    CPU::CPU(const std::string &fileName) : pc_(DRAM_BASE), bus_ {fileName}
    {
        lastInstAddr_ = bus_.getLastInstr();
        mode_         = Machine;
    }

    std::optional<u64> CPU::getRegValueByName(const std::string &name)
    {
        auto it = std::find(Registers::RVABI.cbegin(), Registers::RVABI.cend(), name);
        if (it != Registers::RVABI.cend())
        {
            auto index = std::distance(Registers::RVABI.cbegin(), it);
            return registers_.read(index);
        }

        auto mIt = CSRInterface::csrAddrs_.find(name);
        if (mIt != CSRInterface::csrAddrs_.cend())
            return csrs_.read(mIt->second);

        return std::nullopt;
    }

    void CPU::steps()
    {
        while (!checkEndProgram())
        {
            u32 inst = fetch();

            std::unique_ptr<InstructionFormat> instFormat = nullptr;
            try
            {
                instFormat = decode(inst);
            }
            catch (const char *dec_exc)
            {
                std::cout << "Exception in decoding stage: " << dec_exc << "\n";
                break;
            }
            assert(instFormat != nullptr);

            try
            {
                execute(instFormat);
            }
            catch (const char *exec_exc)
            {
                std::cout << "Exception in execute stage: " << exec_exc << std::endl;
                break;
            }

            try
            {
                memoryAccess(instFormat);
            }
            catch (const char *mem_exc)
            {
                std::cout << "Exception in memory stage: " << mem_exc << std::endl;
                break;
            }

            writeBack(instFormat);
            try
            {
                pc_ = this->moveNextInst(instFormat);
            }
            catch (char *const wb_exception)
            {
                std::cout << "Exception in write back stage: " << wb_exception << std::endl;
                break;
            }

            dumpRegisters();
            dumpCSRs();
            dumpPC();
        }
    }

    AddrType CPU::fetch() { return bus_.readData(pc_, DataSizeType::Word); }

    std::unique_ptr<InstructionFormat> CPU::decode(const InstSizeType inst)
    {
        std::unique_ptr<InstructionFormat> instFormat = nullptr;
        u8 opcode = BitsManipulation::takeBits(inst, 0, LAST_OPCODE_DIGIT);

        auto op = static_cast<OpcodeType>(opcode);
        switch (op)
        {
            case OpcodeType::Lui:     instFormat = std::make_unique<Lui>(inst, pc_); break;
            case OpcodeType::Auipc:   instFormat = std::make_unique<Auipc>(inst, pc_); break;
            case OpcodeType::Jalr:    instFormat = std::make_unique<Jris>(inst, pc_); break;
            case OpcodeType::Jal:     instFormat = std::make_unique<Jal>(inst, pc_); break;
            case OpcodeType::Load:    instFormat = std::make_unique<Load>(inst, pc_); break;
            case OpcodeType::Store:   instFormat = std::make_unique<Store>(inst, pc_); break;
            case OpcodeType::Branch:  instFormat = std::make_unique<Branch>(inst, pc_); break;
            case OpcodeType::Immop:   instFormat = std::make_unique<ImmOp>(inst, pc_); break;
            case OpcodeType::Immop64: instFormat = std::make_unique<ImmOp64>(inst, pc_); break;
            case OpcodeType::Op:      instFormat = std::make_unique<Op>(inst, pc_); break;
            case OpcodeType::Op64:    instFormat = std::make_unique<Op64>(inst, pc_); break;
            case OpcodeType::Fence:   instFormat = std::make_unique<Fence>(inst, pc_); break;
            case OpcodeType::System:  {
                u8 func3 = BitsManipulation::takeBits(inst, 12, 14);
                u8 func7 = BitsManipulation::takeBits(inst, 25, 31);

                if (func3 != 0)
                    instFormat = std::make_unique<CSR>(inst, pc_);

                if (func3 == 0 && func7 != 0)
                    instFormat = std::make_unique<ModeRet>(inst, pc_, *this);
                break;
            }

            default: {
                std::cerr << "No opcode matches\n";
                abort();
            }
        }
        return instFormat;
    }

    void CPU::execute(const std::unique_ptr<InstructionFormat> &instFormat)
    {
        instFormat->readRegister(registers_);
        instFormat->readCsr(csrs_);
        try
        {
            instFormat->execution();
        }
        catch (const char *exc)
        {
            throw exc;
        }
    }

    void CPU::memoryAccess(const std::unique_ptr<InstructionFormat> &instFormat)
    {
        instFormat->accessMemory(bus_);
        instFormat->writeCsr(csrs_);
    }

    void CPU::writeBack(const std::unique_ptr<InstructionFormat> &instFormat)
    {
        instFormat->writeBack(registers_);
    }

    InstSizeType CPU::moveNextInst(const std::unique_ptr<InstructionFormat> &instFormat)
    {
        return instFormat->moveNextInst();
    }

    void CPU::dumpRegisters()
    {
        fmt::print("{:-^100}\n", "registers");
        std::string regFormat = "{:3}({:^4}) = {:#018x}";
        auto outFormat        = fmt::format("{0}   {0}   {0}   {0}\n", regFormat);
        for (size_t i = 0; i < 32; i += 4)
        {
            auto i0 = fmt::format("x{}", i);
            auto i1 = fmt::format("x{}", i + 1);
            auto i2 = fmt::format("x{}", i + 2);
            auto i3 = fmt::format("x{}", i + 3);
            auto v0 = registers_.read(i);
            auto v1 = registers_.read(i + 1);
            auto v2 = registers_.read(i + 2);
            auto v3 = registers_.read(i + 3);
            fmt::vprint(outFormat,
                        fmt::make_format_args(i0,
                                              Registers::RVABI[i],
                                              v0,

                                              i1,
                                              Registers::RVABI[i + 1],
                                              v1,

                                              i2,
                                              Registers::RVABI[i + 2],
                                              v2,

                                              i3,
                                              Registers::RVABI[i + 3],
                                              v3));
        }
    }

    void CPU::dumpCSRs() const { this->csrs_.dumpCSRs(); }

    void CPU::dumpPC() const
    {
        fmt::print("{:-^100}\n", "PC");
        fmt::print("PC = {:#x}\n", pc_);
        fmt::print("{:-^100}\n", "");
    }

}    // namespace rvemu
