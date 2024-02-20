#include "CPU.hpp"
#include "Exception.h"
#include <cstdint>
#include <iostream>
#include <iomanip>    // 用于格式化输出
#include <optional>

uint64_t CPU::load(uint64_t addr, uint64_t size)
{
    try
    {
        return bus.load(addr, size);
    }
    catch (const RVEmuException &e)
    {
        std::cerr << "Exception load: " << e << std::endl;
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
        std::cerr << "Exception store: " << e << std::endl;
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
        std::cerr << "Exception fetch: " << e << std::endl;
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

        std::cout << "Executing instruction: 0x" << std::hex << inst << std::dec << std::endl;

        // execute stage
        switch (opcode)
        {
            case 0x13: {    // addi
                int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
                std::cout << "ADDI: x" << rd << " = x" << rs1 << " + " << imm
                          << std::endl;
                regs[rd] = regs[rs1] + imm;
                return updatePC();
            }
            case 0x33: {    // add
                std::cout << "ADD: x" << rd << " = x" << rs1 << " + " << rs2
                          << std::endl;
                regs[rd] = regs[rs1] + regs[rs2];
                return updatePC();
            }
            default:
                throw RVEmuException(RVEmuException::Type::IllegalInstruction, opcode);
        }
    }
    catch (const RVEmuException &e)
    {
        std::cerr << "Exception execute : " << e << std::endl;
        return std::nullopt;
    }
}

void CPU::dumpRegisters()
{
    std::cout << std::setw(80) << std::setfill('-') << "" << std::endl;    // 打印分隔线
    std::cout << std::setfill(' ');    // 重置填充字符
    for (size_t i = 0; i < 32; i += 4)
    {
        std::cout << std::setw(4) << "x" << i << "(" << RVABI[i] << ") = " << std::hex
                  << std::setw(16) << std::setfill('0') << regs[i] << " " << std::setw(4)
                  << "x" << i + 1 << "(" << RVABI[i + 1] << ") = " << std::setw(16)
                  << regs[i + 1] << " " << std::setw(4) << "x" << i + 2 << "("
                  << RVABI[i + 2] << ") = " << std::setw(16) << regs[i + 2] << " "
                  << std::setw(4) << "x" << i + 3 << "(" << RVABI[i + 3]
                  << ") = " << std::setw(16) << regs[i + 3] << std::endl;
    }
}

void CPU::dumpPC() const
{
    std::cout << std::setw(80) << std::setfill('-') << "" << std::endl;
    std::cout << "PC register" << std::endl;
    std::cout << "PC = " << std::hex << pc << std::dec << std::endl;
    std::cout << std::setw(80) << std::setfill('-') << "" << std::endl;
}
