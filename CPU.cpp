#include "CPU.hpp"
#include "InstructionExecutor.hpp"
#include "Log.hpp"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <optional>
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

        LOG(ERROR, fmt::format("Load failed Invalid address: {:#x} ", addr));
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
            LOG(ERROR, "Store failed.")

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

        LOG(ERROR, fmt::format("Fetch failed. Invalid PC: {:#x}", pc))
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

    void CPU::dumpRegisters()
    {
        LOG(INFO, "Dumping register state:");

        fmt::print("{:-^100}\n", "registers");
        for (size_t i = 0; i < 32; i += 1)
        {
            fmt::print("x{}{:<6} = {:#016x} {:<3}",
                       i,
                       "(" + RVABI[i] + ")",
                       regs[i],
                       " ");
            if ((i + 1) % 4 == 0)
                fmt::print("\n");
        }
    }

    void CPU::dumpPC() const
    {
        fmt::print("{:-^100}\n", "PC");
        LOG(INFO, "Dumping PC register state:");
        fmt::print("PC = {:#x}\n", pc);
        fmt::print("{:-^100}\n", "");
    }

    uint64_t CPU::getRegValueByName(const std::string &regName)
    {
        auto it = std::find(RVABI.begin(), RVABI.end(), regName);
        if (it != RVABI.end())
        {
            int index = std::distance(RVABI.begin(), it);
            return regs[index];
        }
        else
            throw std::invalid_argument("Invalid register name: " + regName);
    }

}    // namespace rvemu
