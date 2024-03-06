#include "Cpu.hpp"

#include "BitsManipulation.hpp"
#include "RVEmu.hpp"
#include "instructions/Iformat.hpp"
#include "instructions/InstFormat.hpp"
#include "instructions/Rformat.hpp"

#include <fmt/core.h>

namespace rvemu
{
    CPU::CPU(const std::string &fileName) : pc_(DRAM_BASE), bus_ {fileName}
    {
        lastInstAddr_ = bus_.getLastInstr();
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
            case OpcodeType::Jalr:  instFormat = std::make_unique<Jris>(inst, pc_); break;
            case OpcodeType::Immop: instFormat = std::make_unique<ImmOp>(inst, pc_); break;
            case OpcodeType::Op:    instFormat = std::make_unique<Op>(inst, pc_); break;

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

    void CPU::dumpPC() const
    {
        fmt::print("{:-^100}\n", "PC");
        fmt::print("PC = {:#x}\n", pc_);
        fmt::print("{:-^100}\n", "");
    }

}    // namespace rvemu
