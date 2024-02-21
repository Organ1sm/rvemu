#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include "fmt/core.h"
#include "InstructionExecutor.hpp"
#include "CPU.hpp"

namespace std
{
    template <>
    struct hash<std::tuple<uint32_t, uint32_t>>
    {
        std::size_t operator()(const std::tuple<uint32_t, uint32_t> &t) const
        {
            return hash<uint32_t>()(std::get<0>(t)) ^ hash<uint32_t>()(std::get<1>(t));
        }
    };
}    // namespace std

namespace rvemu
{
    std::optional<uint64_t> executeAddI(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t imm  = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        fmt::print("ADDI: x{} = x{} + {}\n", rd, rs1, imm);
        cpu.regs[rd] = cpu.regs[rs1] + imm;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSlli(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t imm  = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        fmt::print("SLLI: x{} = x{} << {}\n", rd, rs1, (imm & 0x3f));
        cpu.regs[rd] = cpu.regs[rs1] << (imm & 0x3f);

        return cpu.updatePC();
    }


    std::optional<uint64_t> executeSlti(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t imm  = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        fmt::print("SLTI: x{} = (x{} < {}) ? 1 : 0\n", rd, rs1, (imm & 0x3f));
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<uint64_t>(imm)) ? 1 : 0;
        return cpu.updatePC();
    }


    std::optional<uint64_t> executeSltiu(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t imm  = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        fmt::print("SLTIU: x{} = (x{} < {}) ? 1 : 0\n", rd, rs1, (imm & 0x3f));
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<unsigned int>(imm)) ? 1 : 0;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeXori(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;
        int64_t imm  = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        fmt::print("XORI: x{} = x{} ^ {}\n", rd, rs1, (imm & 0x3f));
        cpu.regs[rd] = cpu.regs[rs1] ^ imm;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSrli(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm =
            static_cast<int32_t>((static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        // "The shift amount is encoded in the lower 6 bits of the I-immediate field for
        // RV64I."
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        fmt::print("SRLI: x{} = x{} >> {}\n", rd, rs1, shamt);
        cpu.regs[rd] = cpu.regs[rs1] >> shamt;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSrai(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>(
            (static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        // 在 I 类型指令中，immediate 字段（I-immediate field）通常用来表示一个立即数，
        // 而对于右移类指令，如算术右移指令（srai），这个立即数的低6位通常用来表示右移的位数。
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        fmt::print("SRAI: x{} = x{} >> {} (arithmetic right shift)\n", rd, rs1, shamt);
        cpu.regs[rd] =
            static_cast<uint64_t>(static_cast<int64_t>(cpu.regs[rs1]) >> shamt);
        return cpu.updatePC();
    }

    std::optional<uint64_t> executefunct70X5(CPU &cpu, uint32_t inst)
    {
        uint32_t funct7 = (inst & 0xfe000000) >> 25;
        fmt::print("Executing srli or srai funct7: {:#x}\n", funct7);
        switch (funct7)
        {
            // srli
            case 0x00: {
                return executeSrli(cpu, inst);
            }
            // srai
            case 0x20: {
                return executeSrai(cpu, inst);
            }
            default: return std::nullopt;
        }
    }

    std::optional<uint64_t> executeOri(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>(
            (static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        fmt::print("ORI: x{} = x{} | {}\n", rd, rs1, imm);
        cpu.regs[rd] = cpu.regs[rs1] | imm;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAndi(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>(
            (static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        fmt::print("ANDI: x{} = x{} & {}\n", rd, rs1, imm);
        cpu.regs[rd] = cpu.regs[rs1] & imm;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAdd(CPU &cpu, uint32_t inst)
    {
        uint32_t rd  = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;

        fmt::print("ADD: x{} = x{} + x{}\n", rd, rs1, rs2);
        cpu.regs[rd] = cpu.regs[rs1] + cpu.regs[rs2];
        return cpu.updatePC();
    }


    std::optional<uint64_t> InstructionExecutor::execute(CPU &cpu, uint32_t inst)
    {
        uint32_t opcode = inst & 0x7f;
        uint32_t funct3 = (inst >> 12) & 0x7;

        // x0 is hardwired zero
        cpu.regs[0] = 0;

        fmt::print("Executing instruction: {:#x}, funct3: {:#x} \n", inst, funct3);

        std::unordered_map<std::tuple<uint32_t, uint32_t>,
                           std::function<std::optional<uint64_t>(CPU &, uint32_t)>>
            instructionMap = {
                {std::make_tuple(0x13, 0x0), executeAddI     },
                {std::make_tuple(0x13, 0x1), executeSlli     },
                {std::make_tuple(0x13, 0x2), executeSlti     },
                {std::make_tuple(0x13, 0x3), executeSltiu    },
                {std::make_tuple(0x13, 0x4), executeXori     },
                {std::make_tuple(0x13, 0x5), executefunct70X5},
                {std::make_tuple(0x13, 0x6), executeOri      },
                {std::make_tuple(0x13, 0x7), executeAndi     },
                {std::make_tuple(0x33, 0x0), executeAdd      },
        };

        auto it = instructionMap.find({opcode, funct3});
        if (it != instructionMap.end())
        {
            return it->second(cpu, inst);
        }
        return std::nullopt;    // 确保所有可能的执行路径都有明确的返回值
    }


}    // namespace rvemu
