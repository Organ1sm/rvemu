#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include "InstructionExecutor.hpp"
#include "CPU.hpp"
#include "Log.hpp"

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

    template <>
    struct hash<std::tuple<uint32_t, uint32_t, uint32_t>>
    {
        size_t operator()(const std::tuple<uint32_t, uint32_t, uint32_t> &key) const
        {
            // 自定义哈希函数的实现
            size_t hash_value = std::hash<unsigned int> {}(std::get<0>(key));
            hash_value ^= std::hash<unsigned int> {}(std::get<1>(key)) + 0x9e3779b9 +
                          (hash_value << 6) + (hash_value >> 2);
            hash_value ^= std::hash<unsigned int> {}(std::get<2>(key)) + 0x9e3779b9 +
                          (hash_value << 6) + (hash_value >> 2);
            return hash_value;
        }
    };
}    // namespace std

namespace rvemu
{
    std::tuple<uint32_t, uint32_t, uint32_t> interpretInst(uint32_t inst)
    {
        return {
            (inst >> 7) & 0x1f,     // rd
            (inst >> 15) & 0x1f,    // rs1
            (inst >> 20) & 0x1f,    // rs2
        };
    }

    std::optional<uint64_t> executeFence(CPU &cpu, uint32_t inst)
    {
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeLb(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LB: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 8);
        if (val.has_value())
        {
            cpu.regs[rd] = static_cast<uint64_t>(static_cast<int8_t>(val.value() & 0xff));
            return cpu.updatePC();
        }

        LOG(ERROR, "LB FAILED!");
        return std::nullopt;
    }


    std::optional<uint64_t> executeLh(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LH: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 16);
        if (val.has_value())
        {
            cpu.regs[rd] =
                static_cast<uint64_t>(static_cast<int16_t>(val.value() & 0xffff));
            return cpu.updatePC();
        }

        LOG(ERROR, "LH FAILED!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeLw(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LW: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 32);
        if (val.has_value())
        {
            cpu.regs[rd] =
                static_cast<uint64_t>(static_cast<int32_t>(val.value() & 0xffffffff));
            return cpu.updatePC();
        }

        LOG(ERROR, "LW FAILED!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeLd(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LD: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 64);
        if (val.has_value())
        {
            cpu.regs[rd] = val.value();
            return cpu.updatePC();
        }

        LOG(ERROR, "LD FAILED!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeLbu(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LBU: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 8);
        if (val.has_value())
        {
            cpu.regs[rd] = val.value() & 0xff;    // zero extend
            return cpu.updatePC();
        }

        LOG(ERROR, "LBU FAILED!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeLhu(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LHU: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 16);
        if (val.has_value())
        {
            cpu.regs[rd] = val.value() & 0xffff;    // zero extend
            return cpu.updatePC();
        }

        LOG(ERROR, "LHU FAILED!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeLwu(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("LWU: x{} = MEM[x{} + {}]", rd, rs1, imm));

        auto val = cpu.load(addr, 32);
        if (val.has_value())
        {
            cpu.regs[rd] = val.value() & 0xffffffff;    // zero extend
            return cpu.updatePC();
        }

        LOG(ERROR, "LWU FAILED!");
        return std::nullopt;
    }

    int64_t getStoreImm(uint32_t inst)
    {
        return ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
               ((inst >> 7) & 0x1f);
    }

    std::optional<uint64_t> executeSb(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = getStoreImm(inst);
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, fmt::format("SB: x{} = x{} + {} addr: {}", rd, rs1, imm, addr));

        bool isSuc = cpu.store(addr, 8, cpu.regs[rs2]);
        if (isSuc)
        {
            LOG(INFO, "SB SUCCESS!")
            return cpu.updatePC();
        }

        LOG(ERROR, "SB FAILED!")
        return std::nullopt;
    }

    std::optional<uint64_t> executeStoreByte(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = getStoreImm(inst);
        uint64_t addr       = cpu.regs[rs1] + imm;

        LOG(INFO, "SB: x", rd, " = x", rs1, " + ", imm, " addr: ", addr);

        bool isSuc = cpu.store(addr, 8, cpu.regs[rs2]);
        if (isSuc)
        {
            LOG(INFO, "SB SUCCESS!");
            return cpu.updatePC();
        }

        LOG(ERROR, "SB FAIL!");
        return std::nullopt;
    }

    std::optional<uint64_t> executeStoreHalf(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = getStoreImm(inst);
        uint64_t addr       = cpu.regs[rs1] + imm;

        cpu.store(addr, 16, cpu.regs[rs2]);
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeStoreWord(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = getStoreImm(inst);
        uint64_t addr       = cpu.regs[rs1] + imm;

        cpu.store(addr, 32, cpu.regs[rs2]);
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeStoreDouble(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = getStoreImm(inst);
        uint64_t addr       = cpu.regs[rs1] + imm;

        cpu.store(addr, 64, cpu.regs[rs2]);
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeStore(CPU &cpu, uint32_t inst)
    {
        auto [rs1, rs2, funct3] = interpretInst(inst);
        int64_t imm             = getStoreImm(inst);
        uint64_t addr           = cpu.regs[rs1] + imm;

        switch (funct3)
        {
            case 0x0: {
                // sb
                cpu.store(addr, 8, cpu.regs[rs2]);
                break;
            }
            case 0x1: {
                // sh
                cpu.store(addr, 16, cpu.regs[rs2]);
                break;
            }
            case 0x2: {
                // sw
                cpu.store(addr, 32, cpu.regs[rs2]);
                break;
            }
            case 0x3: {
                // sd
                cpu.store(addr, 64, cpu.regs[rs2]);
                break;
            }
            default:
                // Unreachable, as the funct3 values are known
                break;
        }

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAddI(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("ADDI: x{} = x{} + {}", rd, rs1, imm));
        cpu.regs[rd] = cpu.regs[rs1] + imm;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSlli(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("SLLI: x{} = x{} << {}", rd, rs1, (imm & 0x3f)));
        cpu.regs[rd] = cpu.regs[rs1] << (imm & 0x3f);

        return cpu.updatePC();
    }


    std::optional<uint64_t> executeSlti(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("SLTI: x{} = (x{} < {}) ? 1 : 0", rd, rs1, (imm & 0x3f)));
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<uint64_t>(imm)) ? 1 : 0;

        return cpu.updatePC();
    }


    std::optional<uint64_t> executeSltiu(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("SLTIU: x{} = (x{} < {}) ? 1 : 0", rd, rs1, (imm & 0x3f)));
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<unsigned int>(imm)) ? 1 : 0;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeXori(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("XORI: x{} = x{} ^ {}", rd, rs1, (imm & 0x3f)));
        cpu.regs[rd] = cpu.regs[rs1] ^ imm;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSrli(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = (static_cast<int32_t>(inst & 0xfff00000) >> 20);

        // "The shift amount is encoded in the lower 6 bits of the I-immediate field for
        // RV64I."
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        LOG(INFO, fmt::format("SRLI: x{} = x{} >> {}", rd, rs1, shamt));
        cpu.regs[rd] = cpu.regs[rs1] >> shamt;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSrai(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        uint64_t imm        = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        // 在 I 类型指令中，immediate 字段（I-immediate field）通常用来表示一个立即数，
        // 而对于右移类指令，如算术右移指令（srai），这个立即数的低6位通常用来表示右移的位数。
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        LOG(INFO,
            fmt::format("SRAI: x{} = x{} >> {} (arithmetic right shift)",
                        rd,
                        rs1,
                        shamt));
        cpu.regs[rd] =
            static_cast<uint64_t>(static_cast<int64_t>(cpu.regs[rs1]) >> shamt);

        return cpu.updatePC();
    }

    std::optional<uint64_t> executefunct70X5(CPU &cpu, uint32_t inst)
    {
        uint32_t funct7 = (inst & 0xfe000000) >> 25;

        LOG(INFO, fmt::format("Executing srli or srai funct7: {:#x}", funct7));

        switch (funct7)
        {
            case 0x00: return executeSrli(cpu, inst);
            case 0x20: return executeSrai(cpu, inst);
            default: return std::nullopt;
        }
    }

    std::optional<uint64_t> executeOri(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("ORI: x{} = x{} | {}", rd, rs1, imm));
        cpu.regs[rd] = cpu.regs[rs1] | imm;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAndi(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        LOG(INFO, fmt::format("ANDI: x{} = x{} & {}", rd, rs1, imm));
        cpu.regs[rd] = cpu.regs[rs1] & imm;

        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAdd(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("ADD: x{} = x{} + x{}", rd, rs1, rs2));
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

        std::unordered_map<std::tuple<uint32_t, uint32_t>, ExecuteFunction>
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
