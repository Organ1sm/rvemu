#include "CPU.hpp"
#include "InstructionExecutor.hpp"
#include <cstdint>
#include <optional>
#include "fmt/core.h"

namespace rvemu
{
    std::optional<uint64_t> CPU::load(uint64_t addr, uint64_t size)
    {
        auto val = bus.load(addr, size);
        if (val.has_value())
            return val;

        return std::nullopt;
    }

    bool CPU::store(uint64_t addr, uint64_t size, uint64_t value)
    {
        return bus.store(addr, size, value);
    }

    std::optional<uint32_t> CPU::fetch()
    {
        auto inst = bus.load(pc, 32);
        if (inst.has_value())
            return inst.value();

        return std::nullopt;
    }

    std::optional<uint64_t> CPU::execute(uint32_t inst)
    {
        auto exe = InstructionExecutor::execute(*this, inst);
        if (exe.has_value())
            return exe;

        return std::nullopt;
    }

    void CPU::dumpRegisters()
    {
        fmt::print("{:-^100}\n", "registers");
        for (size_t i = 0; i < 32; i += 1)
        {
        fmt::print("x{}{:<6} = {:#016x} {:<3}", i, "(" + RVABI[i] + ")", regs[i], " ");
            if ((i + 1) % 4 == 0)
                fmt::print("\n");
        }
    }

    void CPU::dumpPC() const
    {
        fmt::print("{:-^100}\n", "PC");
        fmt::print("PC = {:#x}\n", pc);
        fmt::print("{:-^100}\n", "");
    }
}    // namespace rvemu
