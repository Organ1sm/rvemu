#include "CPU.hpp"
#include "InstructionExecutor.hpp"
#include "Log.hpp"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <optional>
#include <string>
#include "fmt/core.h"

namespace rvemu
{
    std::optional<uint64_t> CPU::load(uint64_t addr, uint64_t size)
    {
        auto val = bus.load(addr, size);
        if (val.has_value())
        {
            LOG(INFO, fmt::format("Load success. Value: {:#x} ", val.value()));
            return val;
        }

        LOG(WARNING, fmt::format("Load failed Invalid address: {:#x} ", addr));
        return std::nullopt;
    }

    bool CPU::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        LOG(INFO,
            fmt::format("Storing value {} at address {:#x} with size {} bytes.",
                        value,
                        addr,
                        size));

        auto result = bus.store(addr, size, value);
        if (result)
            LOG(INFO, "Store successful.")
        else
            LOG(WARNING, "Store failed.")

        return result;
    }

    std::optional<uint32_t> CPU::fetch()
    {
        auto inst = bus.load(pc, 32);
        if (inst.has_value())
        {
            LOG(INFO, fmt::format("Instruction fetched: {:#x}", inst.value()))
            return inst.value();
        }

        LOG(WARNING, fmt::format("Fetch failed. Invalid PC: {:#x}", pc))
        return std::nullopt;
    }

    std::optional<uint64_t> CPU::execute(uint32_t inst)
    {
        auto exe = InstructionExecutor::execute(*this, inst);
        if (exe.has_value())
        {
            LOG(INFO, fmt::format("Execution successful. Result: {:#x}", exe.value()))
            return exe;
        }

        LOG(ERROR, "Execution failed.")
        return std::nullopt;
    }


    void CPU::dumpCSRs() { this->csr.dumpCSRs(); }

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
            fmt::vprint(outFormat,
                        fmt::make_format_args(i0,
                                              RVABI[i],
                                              regs[i],
                                              i1,
                                              RVABI[i + 1],
                                              regs[i + 1],
                                              i2,
                                              RVABI[i + 2],
                                              regs[i + 2],
                                              i3,
                                              RVABI[i + 3],
                                              regs[i + 3]));
        }
    }

    void CPU::dumpPC() const
    {
        fmt::print("{:-^100}\n", "PC");
        fmt::print("PC = {:#x}\n", pc);
        fmt::print("{:-^100}\n", "");
    }

    std::optional<uint64_t> CPU::getRegValueByName(const std::string &regName)
    {
        auto it = std::find(RVABI.begin(), RVABI.end(), regName);
        if (it != RVABI.end())
        {
            int index = std::distance(RVABI.begin(), it);
            return regs[index];
        }

        if (regName == "mhartid")
            return csr.load(MHARTID);
        else if (regName == "mstatus")
            return csr.load(MSTATUS);
        else if (regName == "mtvec")
            return csr.load(MTVEC);
        else if (regName == "mepc")
            return csr.load(MEPC);
        else if (regName == "mcause")
            return csr.load(MCAUSE);
        else if (regName == "mtval")
            return csr.load(MTVAL);
        else if (regName == "medeleg")
            return csr.load(MEDELEG);
        else if (regName == "mscratch")
            return csr.load(MSCRATCH);
        else if (regName == "MIP")
            return csr.load(MIP);
        else if (regName == "mcounteren")
            return csr.load(MCOUNTEREN);
        else if (regName == "sstatus")
            return csr.load(SSTATUS);
        else if (regName == "stvec")
            return csr.load(STVEC);
        else if (regName == "sepc")
            return csr.load(SEPC);
        else if (regName == "scause")
            return csr.load(SCAUSE);
        else if (regName == "stval")
            return csr.load(STVAL);
        else if (regName == "sscratch")
            return csr.load(SSCRATCH);
        else if (regName == "SIP")
            return csr.load(SIP);
        else if (regName == "SATP")
            return csr.load(SATP);

        LOG(WARNING, fmt::format("Invalid name: {}", regName));
        return std::nullopt;
    }
}    // namespace rvemu
