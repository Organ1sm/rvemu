#include "Cpu.hpp"

#include "BitsManipulation.hpp"
#include "RVEmu.hpp"
#include "instructions/Iformat.hpp"
#include "instructions/InstFormat.hpp"

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
            uint32_t inst = fetch();

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
        }
    }

    AddrType CPU::fetch() { return bus_.readData(pc_, DataSizeType::Word); }

    std::unique_ptr<InstructionFormat> CPU::decode(const InstSizeType inst)
    {
        std::unique_ptr<InstructionFormat> instFormat = nullptr;
        uint8_t opcode = BitsManipulation::takeBits(inst, 0, LAST_OPCODE_DIGIT);

        auto op = static_cast<OpcodeType>(opcode);
        switch (op)
        {
            case OpcodeType::Jalr:  instFormat = std::make_unique<Jris>(inst, pc_); break;
            case OpcodeType::Immop: instFormat = std::make_unique<ImmOp>(inst, pc_); break;

            default: {
                std::cerr << "No opcode matches\n";
                abort();
            }
        }
        return instFormat;
    }

    void CPU::execute(const std::unique_ptr<InstructionFormat> &is_format)
    {
        is_format->readRegister(registers_);
        is_format->readCsr(csrs_);
        try
        {
            is_format->execution();
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

}    // namespace rvemu
