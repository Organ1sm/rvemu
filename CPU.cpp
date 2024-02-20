#include "CPU.hpp"
#include "Exception.h"
#include <cstdint>
#include <iostream>
#include <optional>
#include "fmt/core.h"

uint64_t CPU::load(uint64_t addr, uint64_t size)
{
    try
    {
        return bus.load(addr, size);
    }
    catch (const RVEmuException &e)
    {
        fmt::print(std::cerr, "Exception load: {}\n", e);
    }
}

void CPU::store(uint64_t addr, uint64_t size, uint64_t value)
{
    try
    {
        bus.store(addr, size, value);
    }
    catch (const RVEmuException &e)
    {
        fmt::print(std::cerr, "Exception load: {}\n", e);
    }
}

uint32_t CPU::fetch()
{
    try
    {
        return bus.load(pc, 32);
    }
    catch (const RVEmuException &e)
    {
        fmt::print(std::cerr, "Exception fetch: {}\n", e);
    }
}

std::optional<uint64_t> CPU::execute(uint32_t inst)
{
    try
    {
        uint32_t opcode = inst & 0x7f;
        uint32_t rd     = (inst >> 7) & 0x1f;
        uint32_t rs1    = (inst >> 15) & 0x1f;
        uint32_t rs2    = (inst >> 20) & 0x1f;
        uint32_t funct3 = (inst >> 12) & 0x7;
        uint32_t funct7 = (inst >> 25) & 0x7f;

        // x0 is hardwired zero
        regs[0] = 0;

        fmt::print("Executing instruction: {:#x}\n", inst);
        switch (opcode)
        {
            case 0x13: {    // addi
                int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
                fmt::print("ADDI: x{} = x{} + {}\n", rd, rs1, imm);
                regs[rd] = regs[rs1] + imm;
                return updatePC();
            }
            case 0x33: {    // add
                fmt::print("ADD: x{} = x{} + {}\n", rd, rs1, rs2);
                regs[rd] = regs[rs1] + regs[rs2];
                return updatePC();
            }
            default:
                throw RVEmuException(RVEmuException::Type::IllegalInstruction, opcode);
        }
    }
    catch (const RVEmuException &e)
    {
        fmt::print(std::cerr, "Exception execute: {}\n", e);
        return std::nullopt;
    }
}

void CPU::dumpRegisters()
{
    fmt::print("{:-^100}\n", "registers");
    for (size_t i = 0; i < 32; i += 1)
    {
        fmt::print("x{}({}) = {:#016x} {:<3}", i, RVABI[i], regs[i], " ");
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
