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

    std::optional<uint64_t> executeAddW(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("ADDW: x{} = x{} + x{}", rd, rs1, rs2));

        int64_t result =
            static_cast<int32_t>(cpu.regs[rs1]) + static_cast<int32_t>(cpu.regs[rs2]);
        cpu.regs[rd] = static_cast<uint64_t>(result);
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

    std::optional<uint64_t> executeSll(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("SLL: x{} = x{} << {}", rd, rs1, rs2));

        cpu.regs[rd] = cpu.regs[rs1] << cpu.regs[rs2];
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSlt(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("SLT: x{} = (x{} < x{}) ? 1 : 0", rd, rs1, rs2));
        LOG(INFO,
            fmt::format("Values: x{} = {}, x{} = {}",
                        rs1,
                        cpu.regs[rs1],
                        rs2,
                        cpu.regs[rs2]));

        cpu.regs[rd] =
            (static_cast<int64_t>(cpu.regs[rs1]) < static_cast<int64_t>(cpu.regs[rs2])) ?
                1 :
                0;
        return cpu.updatePC();
    }


    std::optional<uint64_t> executeXor(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("XOR: x{} = x{} ^ x{}", rd, rs1, rs2));

        cpu.regs[rd] = cpu.regs[rs1] ^ cpu.regs[rs2];
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSrl(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("SRL: x{} = x{} >> x{}", rd, rs1, rs2));

        cpu.regs[rd] = cpu.regs[rs1] >> cpu.regs[rs2];
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeSra(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO,
            fmt::format("SRA: x{} = x{} >> x{} (arithmetic right shift)", rd, rs1, rs2));

        cpu.regs[rd] =
            static_cast<uint64_t>(static_cast<int64_t>(cpu.regs[rs1]) >> cpu.regs[rs2]);
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeOr(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("OR: x{} = x{} | x{}", rd, rs1, rs2));

        cpu.regs[rd] = cpu.regs[rs1] | cpu.regs[rs2];
        return cpu.updatePC();
    }


    std::optional<uint64_t> executeAnd(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);

        LOG(INFO, fmt::format("OR: x{} = x{} & x{}", rd, rs1, rs2));

        cpu.regs[rd] = cpu.regs[rs1] & cpu.regs[rs2];
        return cpu.updatePC();
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


    std::optional<uint64_t> executeLui(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        auto imm =
            static_cast<uint64_t>(inst & 0xfffff000);    // Extract the upper 20 bits

        LOG(INFO, fmt::format("LUI: x{} = {}", rd, imm));

        cpu.regs[rd] = imm;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeAUIPC(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfffff000);

        LOG(INFO, fmt::format("AUIPC: x{} = pc + {}", rd, imm));

        cpu.regs[rd] = cpu.pc + imm;
        return cpu.updatePC();
    }

    std::optional<uint64_t> executeJAL(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        auto imm = ((inst & 0x80000000) ? 0xfff00000 : 0) | ((inst >> 20) & 0x7fe) |
                   ((inst >> 9) & 0x800) | ((inst >> 12) & 0xff);

        LOG(INFO, fmt::format("JAL: x{} = pc + 4; pc = pc + {}", rd, imm));

        cpu.regs[rd] = cpu.pc + 4;
        return cpu.pc + imm;
    }


    std::optional<uint64_t> executeJALR(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm         = static_cast<int32_t>(inst & 0xfff00000) >> 20;

        uint64_t t     = cpu.pc + 4;
        uint64_t newPC = (cpu.regs[rs1] + imm) & ~1;

        LOG(INFO, fmt::format("JALR: x{} = pc + 4; pc = (x{} + {}) & -1", rd, rs1, imm));
        cpu.regs[rd] = t;
        return newPC;
    }

    std::optional<uint64_t> executeBEQ(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        int64_t imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                            ((inst & 0x80) << 4) |
                                            ((inst >> 20) & 0x7E0) | ((inst >> 7) & 0x1E))
                                           << 1) >>
                      1;    // Sign-extend

        if (cpu.regs[rs1] == cpu.regs[rs2])
        {
            LOG(INFO, fmt::format("BEQ: pc = pc + {}", imm));
            return cpu.pc + imm;
        }

        return std::nullopt;
    }

    std::optional<uint64_t> executeBNE(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                         ((inst & 0x80) << 4) | ((inst >> 20) & 0x7E0) |
                                         ((inst >> 7) & 0x1E))
                                        << 1) >>
                   1;    // Sign-extend

        if (cpu.regs[rs1] != cpu.regs[rs2])
        {
            LOG(INFO, fmt::format("BNE: pc = pc + {}", imm));
            return cpu.pc + imm;
        }

        return std::nullopt;
    }


    /**
     * @brief Executes the BLT (Branch if Less Than) instruction in the RISC-V instruction
     * set.
     *
     * The BLT instruction compares two registers and, if the first is less than the
     * second, branches to an instruction located a certain distance away.
     *
     * @param cpu The current state of the CPU, including registers and other relevant
     * data.
     * @param inst The 32-bit binary representation of the instruction to execute.
     *
     * @return If the branch is taken, returns the new program counter value. If the
     * branch is not taken, returns std::nullopt.
     */
    std::optional<uint64_t> executeBLT(CPU &cpu, uint32_t inst)
    {
        // Unpack the instruction into its constituent parts
        auto [rd, rs1, rs2] = interpretInst(inst);

        // Calculate the branch offset, taking into account that it's a signed value
        int64_t imm = static_cast<int64_t>(
            (static_cast<int64_t>(inst) & 0x80000000 ? 0xFFFFF000LL : 0) |
            ((static_cast<uint64_t>(inst) & 0x80) << 4) | ((inst >> 20) & 0x7E0) |
            ((inst >> 7) & 0x1E));

        // If the value in rs1 is less than the value in rs2, branch to the calculated
        // offset
        if (static_cast<int64_t>(cpu.regs[rs1]) < static_cast<int64_t>(cpu.regs[rs2]))
        {
            LOG(INFO, "BLT: pc = pc + ", imm);
            return cpu.pc + imm;
        }

        // If the branch is not taken, return std::nullopt
        return std::nullopt;
    }


    std::optional<uint64_t> executeBGE(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                         ((inst & 0x80) << 4) | ((inst >> 20) & 0x7E0) |
                                         ((inst >> 7) & 0x1E))
                                        << 1) >>
                   1;    // Sign-extend

        if (static_cast<int64_t>(cpu.regs[rs1]) >= static_cast<int64_t>(cpu.regs[rs2]))
        {
            LOG(INFO, fmt::format("BGE: pc = pc + {}", imm));
            return cpu.pc + imm;
        }

        return std::nullopt;
    }

    std::optional<uint64_t> executeBGEU(CPU &cpu, uint32_t inst)
    {
        auto [rd, rs1, rs2] = interpretInst(inst);
        auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                         ((inst & 0x80) << 4) | ((inst >> 20) & 0x7E0) |
                                         ((inst >> 7) & 0x1E))
                                        << 1) >>
                   1;    // Sign-extend

        if (cpu.regs[rs1] >= cpu.regs[rs2])
        {
            LOG(INFO, fmt::format("BGEU: pc = pc + {}", imm));
            return cpu.pc + imm;
        }

        return std::nullopt;
    }

    std::optional<uint64_t> InstructionExecutor::execute(CPU &cpu, uint32_t inst)
    {
        uint32_t opcode = inst & 0x0000007f;
        uint32_t funct3 = (inst & 0x00007000) >> 12;
        uint32_t funct7 = (inst & 0xfe000000) >> 25;

        // x0 is hardwired zero
        cpu.regs[0] = 0;

        LOG(INFO, fmt::format("Instruction: {:#x}", inst))
        LOG(INFO, fmt::format("Executing instruction with opcode: {:#x}", opcode));


        std::unordered_map<uint32_t, ExecuteFunction> opcodeTable = {
            {0x17, executeAUIPC},
            {0x37, executeLui  },
            {0x67, executeJALR },
            {0x6f, executeJAL  },
        };


        // Look up the corresponding execute function in the opcodeTable
        auto executeFunc = opcodeTable.find(opcode);

        // If found, call the execute function with the given instruction
        if (executeFunc != opcodeTable.end())
        {
            auto result = executeFunc->second(cpu, inst);
            if (result.has_value())
            {
                LOG(INFO,
                    fmt::format("Instruction executed successfully. New PC: {:#x}",
                                result.value()));
            }
            else
            {
                LOG(ERROR, "Failed to execute instruction.");
            }
            return result;
        }

        LOG(INFO, fmt::format("funct3: {:#x}", funct3));

        std::unordered_map<std::tuple<uint32_t, uint32_t>, ExecuteFunction>
            instructionMap = {
                {std::make_tuple(0x03, 0x0), executeLb         },
                {std::make_tuple(0x03, 0x1), executeLh         },
                {std::make_tuple(0x03, 0x2), executeLw         },
                {std::make_tuple(0x03, 0x3), executeLd         },
                {std::make_tuple(0x03, 0x4), executeLbu        },
                {std::make_tuple(0x03, 0x5), executeLhu        },
                {std::make_tuple(0x03, 0x6), executeLwu        },
                {std::make_tuple(0x0f, 0x0), executeFence      },
                {std::make_tuple(0x13, 0x0), executeAddI       },
                {std::make_tuple(0x13, 0x1), executeSlli       },
                {std::make_tuple(0x13, 0x2), executeSlti       },
                {std::make_tuple(0x13, 0x3), executeSltiu      },
                {std::make_tuple(0x13, 0x4), executeXori       },
                {std::make_tuple(0x13, 0x6), executeOri        },
                {std::make_tuple(0x13, 0x7), executeAndi       },
                {std::make_tuple(0x19, 0x7), executeSb         },
                {std::make_tuple(0x23, 0x0), executeStoreByte  },
                {std::make_tuple(0x23, 0x3), executeStoreDouble},
                {std::make_tuple(0x63, 0x1), executeBEQ        },
        };

        auto it = instructionMap.find({opcode, funct3});
        if (it != instructionMap.end())
        {
            auto result = it->second(cpu, inst);
            if (result.has_value())
            {
                LOG(INFO,
                    fmt::format("Instruction executed successfully. New PC: {:#x}",
                                result.value()));
            }
            else
            {
                LOG(ERROR, "Failed to execute instruction.");
            }

            return result;
        }


        LOG(INFO, fmt::format("funct7: {:#x}", funct7));

        std::unordered_map<std::tuple<uint32_t, uint32_t, uint32_t>, ExecuteFunction>
            instruction2Map = {
                {std::make_tuple(0x13, 0x5, 0x00), executeSrli},
                {std::make_tuple(0x13, 0x5, 0x20), executeSrai},
                {std::make_tuple(0x33, 0x0, 0x00), executeAdd },
                {std::make_tuple(0x33, 0x1, 0x00), executeSll },
                {std::make_tuple(0x33, 0x2, 0x00), executeSlt },
                {std::make_tuple(0x33, 0x4, 0x00), executeXor },
                {std::make_tuple(0x33, 0x5, 0x00), executeSrl },
                {std::make_tuple(0x33, 0x5, 0x20), executeSra },
                {std::make_tuple(0x33, 0x6, 0x00), executeOr  },
                {std::make_tuple(0x33, 0x7, 0x00), executeAnd },
                {std::make_tuple(0x3b, 0x0, 0x00), executeAddW},
        };

        LOG(INFO, fmt::format("Executing srli or srai funct7: {:#x}", funct7));

        auto it1 = instruction2Map.find({opcode, funct3, funct7});
        if (it1 != instruction2Map.end())
        {
            auto result = it1->second(cpu, inst);
            if (result.has_value())
            {
                LOG(INFO,
                    fmt::format("Instruction executed successfully. New PC: {:#x}",
                                result.value()));
            }
            else
            {
                LOG(ERROR, "Failed to execute instruction.");
            }
            return result;
        }
        else
        {
            LOG(ERROR,
                fmt::format(
                    "Unsupported instruction: {:#x}, opcode: {:#x}, funct3: {:#x}, funct7: {:#x}",
                    inst,
                    opcode,
                    funct3,
                    funct7));
            return std::nullopt;
        }
    }

}    // namespace rvemu
