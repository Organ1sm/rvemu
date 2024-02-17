#include "CPU.hpp"
#include <cstdint>
#include <iostream>
#include <iomanip>    // 用于格式化输出

uint32_t CPU::fetch()
{
    std::size_t index = static_cast<std::size_t>(pc);
    uint32_t inst     = static_cast<uint32_t>(dram[index]) |
                    (static_cast<uint32_t>(dram[index + 1]) << 8) |
                    (static_cast<uint32_t>(dram[index + 2]) << 16) |
                    (static_cast<uint32_t>(dram[index + 3]) << 24);

    return inst;
}

void CPU::execute(uint32_t inst)
{
    uint32_t opcode = inst & 0x7f;
    uint32_t rd     = (inst >> 7) & 0x1f;
    uint32_t rs1    = (inst >> 15) & 0x1f;
    uint32_t rs2    = (inst >> 20) & 0x1f;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t funct7 = (inst >> 25) & 0x7f;

    // x0 is hardwired zero
    regs[0] = 0;

    // execute stage
    switch (opcode)
    {
        case 0x13: {    // addi
            int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
            regs[rd]    = regs[rs1] + imm;
            break;
        }
        case 0x33: {    // add
            regs[rd] = regs[rs1] + regs[rs2];
            break;
        }
        default:
            std::cerr << "Invalid opcode: " << std::hex << opcode << std::endl;
            break;
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
