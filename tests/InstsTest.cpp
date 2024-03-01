#include "testUtil.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>


namespace rvemu
{
    TEST_CASE("RVTests-addi", "Test addi Instructions")
    {
        SECTION("addi instruction with a postive immediate value")
        {
            std::string code = start + "addi x31, x0, 42 \n";
            CPU cpu          = rvHelper(code, "test_addi1", 1);

            REQUIRE(cpu.regs[31] == 42);
        }

        SECTION("addi instruction with a negative immediate value")
        {
            std::string code = start + "addi x2, x0, -10 \n";
            CPU cpu          = rvHelper(code, "test_addi2", 1);

            REQUIRE(cpu.regs[2] == -10);
        }

        SECTION("addi instruction with a negative immediate value")
        {
            std::string code = start + "addi x2, x0, 10 \n"
                                       "addi x3, x0, -10 \n"
                                       "addi x4, x0, 0\n";
            CPU cpu          = rvHelper(code, "test_addi3", 3);

            REQUIRE(cpu.regs[2] == 10);
            REQUIRE(cpu.regs[3] == static_cast<uint64_t>(-10));
            REQUIRE(cpu.regs[4] == 0);
        }
    }

    TEST_CASE("RVTests-slli", "Test slli Instruction")
    {
        std::string code = start + "addi x2, x0, 5 \n"    // load 5 into x2
                                   "slli x1, x2, 3\n";    // x1 = x2 << 3

        CPU cpu = rvHelper(code, "test_slli", 2);
        REQUIRE(cpu.regs[1] == (5 << 3));
    }

    TEST_CASE("RVTests-slti", "Test slti Instruction")
    {
        SECTION("test slti instruction")
        {
            std::string code = start + "addi x2, x0, 8 \n"      // x2 = x0 + 8
                                       "slti x1, x2, 10 \n";    // x1 = (x2 < 10) ? 1 : 0

            CPU cpu = rvHelper(code, "test_slti", 2);
            REQUIRE(cpu.regs[1] == 1);
        }


        SECTION("test sltiu instruction")
        {
            std::string code = start + "addi x2, x0, 8 \n"       // x2 = x0 + 8
                                       "sltiu x1, x2, 10 \n";    // x1 = (x2 < 10) ? 1 : 0

            CPU cpu = rvHelper(code, "test_sltiu", 2);
            REQUIRE(cpu.regs[1] == 1);
        }

        SECTION("test sltiu2 instruction")
        {
            std::string code = start + "addi x2, x0, 10 \n"     // x2 = x0 + 10
                                       "sltiu x1, x2, 8 \n";    // x1 = (x2 < 8) ? 1 : 0

            CPU cpu = rvHelper(code, "test_sltiu2", 2);
            REQUIRE(cpu.regs[1] == 0);
        }
    }

    TEST_CASE("RVTests-xori", "Test xori Instruction")
    {
        std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                   "xori x1, x2, 7 \n";    // x1 =  x2 ^ 7

        CPU cpu = rvHelper(code, "test_xori", 2);
        REQUIRE(cpu.regs[1] == 8);
    }

    TEST_CASE("RVTests-sri", "Test sri Instruction")
    {
        SECTION("test srli instruction")
        {
            std::string code =
                start + "addi x2, x0, 32 \n"    // load 32 into x2
                        "srli x1, x2, 3 \n";    // x1 = x2 >> 3 (logical right shift)

            CPU cpu = rvHelper(code, "test_srli", 2);
            REQUIRE(cpu.regs[1] == 4);
        }

        SECTION("test srai instruction")
        {
            std::string code =
                start + "addi x2, x0, -16 \n"    // load -16 into x2
                        "srai x1, x2, 3 \n";     // x1 = x2 >> 3 (arithmetic right shift)

            CPU cpu     = rvHelper(code, "test_srai", 2);
            auto result = static_cast<int64_t>(cpu.regs[1]);
            REQUIRE(cpu.regs[2] == -16);
            REQUIRE(result == -2);
        }
    }

    TEST_CASE("RVTests-add", "Test add Instruction")
    {
        std::string code = start + "addi x2, x0, 10 \n"    // load 10 into x2
                                   "addi x3, x0, 20 \n"    // load 20 into x3
                                   "add x1, x2, x3 \n";    // x1 = x2 + x3

        CPU cpu = rvHelper(code, "test_add", 3);
        REQUIRE(cpu.regs[1] == 30);
    }

    TEST_CASE("RVTests-ori", "Test ori Instruction")
    {
        std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                   "ori x1, x2, 7 \n";     // x1 = x2 | 7

        CPU cpu = rvHelper(code, "test_ori", 2);
        REQUIRE(cpu.regs[1] == (15 | 7));
    }

    TEST_CASE("RVTests-andi", "Test andi Instruction")
    {
        std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                   "andi x1, x2, 7 \n";    // x1 = x2 & 7

        CPU cpu = rvHelper(code, "test_andi", 2);
        REQUIRE(cpu.regs[1] == (15 & 7));
    }


    TEST_CASE("RVTests-Lui", "Test LUI Instruction")
    {
        std::string code = start + "lui a0, 42 \n";
        CPU cpu          = rvHelper(code, "test_lui", 1);

        REQUIRE(cpu.getRegValueByName("a0") == (42 << 12));
    }


    TEST_CASE("RVTests-AUIPC", "Test AUIPC Instruction")
    {
        std::string code = start + "auipc a0, 42 \n";
        CPU cpu          = rvHelper(code, "test_auipc", 1);

        REQUIRE(cpu.getRegValueByName("a0") == DRAM_BASE + (42 << 12));
    }


    TEST_CASE("RVTests-JAL", "Test JAL Instruction")
    {
        std::string code = start + "jal a0, 42 \n";
        CPU cpu          = rvHelper(code, "test_jal", 1);

        REQUIRE(cpu.getRegValueByName("a0") == DRAM_BASE + 4);
        REQUIRE(cpu.pc == DRAM_BASE + 42);
    }

    TEST_CASE("RVTests-JALR", "Test JALR Instruction")
    {
        std::string code = start + "addi a1, zero, 42\n"
                                   "jalr a0, -8(a1)\n";

        CPU cpu = rvHelper(code, "test_jalr", 2);
        REQUIRE(cpu.getRegValueByName("a0") == DRAM_BASE + 8);
        REQUIRE(cpu.pc == 34);
    }

    TEST_CASE("RVTests-Fence", "Test Fence Instruction")
    {
        std::string code = start + "fence\n";
        CPU cpu          = rvHelper(code, "test_fence", 1);

        REQUIRE(cpu.pc == DRAM_BASE + 4);
    }

    TEST_CASE("RVTests-Slt", "Test Slt Instruction")
    {
        std::string code = start + "addi a0, zero, 14 \n"    // Load 14 into a0
                                   "addi a1, zero, 24 \n"    // Load 24 into a1
                                   "slt  a2, a0, a1 \n"      // a2 = (a0 < a1) ? 1 : 0
                                   "slti a3, a0, 42 \n"      // a3 = (a0 < 42) ? 1 : 0
                                   "sltiu a4, a0, 84 \n";    // a4 = (a0 <u 84) ? 1 : 0

        CPU cpu = rvHelper(code, "test_slt", 5);

        REQUIRE(cpu.getRegValueByName("a2") == 1);
        REQUIRE(cpu.getRegValueByName("a3") == 1);
        REQUIRE(cpu.getRegValueByName("a4") == 1);
    }

    TEST_CASE("RVTests-Xor", "Test xor Instruction")
    {
        std::string code = start + "addi a0, zero, 0b10 \n"    // Load 0b10 into a0
                                   "xori a1, a0, 0b01 \n"      // a1 = a0 XOR 0b01
                                   "xor  a2, a1, a1 \n";       // a2 = a1 XOR a1

        CPU cpu = rvHelper(code, "test_xor", 3);

        REQUIRE(cpu.getRegValueByName("a1") == 3);
        REQUIRE(cpu.getRegValueByName("a2") == 0);
    }

    TEST_CASE("RVTests-Or", "Test OR Instruction")
    {
        std::string code = start + "addi a0, zero, 0b10 \n"    // Load 0b10 into a0
                                   "ori  a1, a0, 0b01 \n"      // a1 = a0 OR 0b01
                                   "or   a2, a0, a0 \n";       // a2 = a0 OR a0

        CPU cpu = rvHelper(code, "test_or", 3);

        REQUIRE(cpu.getRegValueByName("a1") == 0b11);
        REQUIRE(cpu.getRegValueByName("a2") == 0b10);
    }

    TEST_CASE("RVTests-And", "Test And Instruction")
    {
        std::string code = start + "addi a0, zero, 0b10 \n"    // Load 0b10 into a0
                                   "andi a1, a0, 0b11 \n"      // a1 = a0 AND 0b11
                                   "and  a2, a0, a1 \n";       // a2 = a0 AND a1

        CPU cpu = rvHelper(code, "test_and", 3);

        REQUIRE(cpu.getRegValueByName("a1") == 0b10);
        REQUIRE(cpu.getRegValueByName("a2") == 0b10);
    }

    TEST_CASE("RVTests-Sll", "Test Sll Instruction")
    {
        std::string code = start + "addi a0, zero, 1 \n"     // Load 1 into a0
                                   "addi a1, zero, 5 \n"     // Load 5 into a1
                                   "sll  a2, a0, a1 \n"      // a2 = a0 << a1
                                   "slli a3, a0, 5 \n"       // a3 = a0 << 5
                                   "addi s0, zero, 64 \n"    // Load 64 into s0
                                   "sll  a4, a0, s0 \n";     // a4 = a0 << s0

        CPU cpu = rvHelper(code, "test_sll", 6);

        REQUIRE(cpu.getRegValueByName("a2") == 1 << 5);
        REQUIRE(cpu.getRegValueByName("a3") == 1 << 5);
        REQUIRE(cpu.getRegValueByName("a4") == 1);
    }

    TEST_CASE("RVTests-SraSrl", "Test SraSrl Instruction")
    {
        std::string code = start + "addi a0, zero, -8; \n"    // Load -8 into a0
                                   "addi a1, zero, 1; \n"     // Load 1 into a1
                                   "sra  a2, a0, a1; \n"      // a2 = a0 >> a1
                                   "srai a3, a0, 2; \n"       // a3 = a0 >> 2
                                   "srli a4, a0, 2; \n"       // a4 = a0 >>> 2
                                   "srl  a5, a0, a1; \n";     // a5 = a0 >>> a1

        CPU cpu = rvHelper(code, "test_sra_srl", 6);

        REQUIRE(cpu.getRegValueByName("a2") == static_cast<uint64_t>(-4));
        REQUIRE(cpu.getRegValueByName("a3") == static_cast<uint64_t>(-2));
        REQUIRE(cpu.getRegValueByName("a4") == static_cast<uint64_t>(-8) >> 2);
        REQUIRE(cpu.getRegValueByName("a5") == static_cast<uint64_t>(-8) >> 1);
    }

    TEST_CASE("RVTests-wordop", "Test word op Instruction")
    {
        std::string code = start +
                           "addi a0, zero, 42 \n"    // Load 42 into a0
                           "lui  a1, 0x7f000 \n"     // Load 0x7f000 into a1
                           "addw a2, a0, a1 \n";     // a2 = a0 + a1 (word operation)

        CPU cpu = rvHelper(code, "test_word_op", 3);

        REQUIRE(cpu.getRegValueByName("a2") == 0x7f00002a);
    }

    TEST_CASE("RVTests-csrrs", "Test csr-rs instruction")
    {
        std::string code =
            start +
            "addi x2, x0, 5 \n"            // Load 5 into x2
            "csrrs x1, mstatus, x2 \n";    // x1 = mstatus; mstatus = mstatus | x2;
        CPU cpu = rvHelper(code, "test_csrrs", 2);

        // Verify if MSTATUS register has the correct value
        REQUIRE(cpu.getRegValueByName("mstatus") == 5);
    }

    TEST_CASE("RVTests-csrrw", "Test csr-rw Instruction")
    {
        SECTION("test csrrw instruction")
        {
            std::string code =
                start + "addi x2, x0, 5 \n"            // Load 5 into x2
                        "csrrw x1, mstatus, x2 \n";    // x1 = mstatus; mstatus = x2;
            CPU cpu = rvHelper(code, "test_csrrw", 2);

            REQUIRE(cpu.getRegValueByName("mstatus") == 5);
        }

        SECTION("test csrrw-complex instruction")
        {
            std::string code =
                start + "addi x2, x0, 5 \n"            // Load 5 into x2
                        "addi x3, x0, 10 \n"           // Load 10 into x3
                        "csrrw x1, mstatus, x2 \n"     // x1 = mstatus; mstatus = x2;
                        "csrrw x4, mstatus, x3 \n";    // x4 = mstatus; mstatus = x3;

            CPU cpu = rvHelper(code, "test_csrrw_complex", 4);

            // verify if x1 and x4 have the correct values
            REQUIRE(cpu.regs[1] == 0);
            REQUIRE(cpu.regs[4] == 5);

            // verify if MSTATUS register has the correct value
            REQUIRE(cpu.getRegValueByName("mstatus") == 10);
        }
    }

    TEST_CASE("RVTests-csrrc", "Test csr-rc Instruction")
    {
        SECTION("test csrrc instruction")
        {
            std::string code =
                start + "addi x2, x0, 5 \n"            // Load 5 into x2
                        "csrrc x1, mstatus, x2 \n";    // x1 = mstatus; mstatus = x2;
            CPU cpu = rvHelper(code, "test_csrrc", 2);

            REQUIRE(cpu.getRegValueByName("mstatus") == 0);
        }

        SECTION("test csrrc-complex instruction")
        {
            std::string code =
                start +
                "addi x2, x0, 5 \n"            // Load 5 into x2
                "addi x3, x0, 10 \n"           // Load 10 into x3
                "csrrc x1, mstatus, x2 \n"     // x1 = mstatus; mstatus = mstatus & ~x2;
                "csrrc x4, mstatus, x3 \n";    // x4 = mstatus; mstatus = mstatus & ~x3;

            CPU cpu = rvHelper(code, "test_csrrc_complex", 4);

            // verify if x1 and x4 have the correct values
            REQUIRE(cpu.regs[1] == 0);
            REQUIRE(cpu.regs[4] == 0);

            // verify if MSTATUS register has the correct value
            REQUIRE(cpu.getRegValueByName("mstatus") == 0);
        }
    }

    TEST_CASE("RVTests-csrrsi", "Test csr-rsi Instruction")
    {
        SECTION("test csr-rsi instruction")
        {
            std::string code =
                start + "csrrsi x1, mstatus, 5 \n";    // x1 = mstatus;
                                                       // mstatus = mstatus | 5;
            CPU cpu = rvHelper(code, "test_csrrc", 1);

            REQUIRE(cpu.getRegValueByName("mstatus") == 5);
        }

        SECTION("test csr-rsi-complex instruction")
        {
            std::string code = start +
                               "csrrsi x1, mstatus, 5 \n"     // x1 = mstatus;
                                                              // mstatus = mstatus |  5;
                               "csrrsi x4, mstatus, 3 \n";    // x4 = mstatus;
                                                              // mstatus = mstatus | 3;

            CPU cpu = rvHelper(code, "test_csrrsi_complex", 2);

            // verify if x1 and x4 have the correct values
            REQUIRE(cpu.regs[1] == 0);
            REQUIRE(cpu.regs[4] == 5);

            // verify if MSTATUS register has the correct value
            REQUIRE(cpu.getRegValueByName("mstatus") == 7);
        }
    }

    TEST_CASE("RVTests-csrrci", "Test csr-rci Instruction")
    {
        SECTION("test csr-rci instruction")
        {
            std::string code =
                start + "csrrci x1, mstatus, 5 \n";    // x1 = mstatus;
                                                       // mstatus = mstatus | ~(1 << 5);
            CPU cpu = rvHelper(code, "test_csrrc", 1);

            REQUIRE(cpu.getRegValueByName("mstatus") == 0);
        }

        SECTION("test csr-rsi-complex instruction")
        {
            std::string code =
                start + "csrrci x1, mstatus, 5 \n"     // x1 = mstatus;
                                                       // mstatus = mstatus | ~(1 << 5);
                        "csrrci x4, mstatus, 3 \n";    // x4 = mstatus;
                                                       // mstatus = mstatus | ~(1 << 3);

            CPU cpu = rvHelper(code, "test_csrrci_complex", 2);

            // verify if x1 and x4 have the correct values
            REQUIRE(cpu.regs[1] == 0);
            REQUIRE(cpu.regs[4] == 0);

            // verify if MSTATUS register has the correct value
            REQUIRE(cpu.getRegValueByName("mstatus") == 0);
        }
    }

    TEST_CASE("RVTests-CSRS", "Test CSRS ")
    {
        std::string code = start + "addi t0, zero, 1 \n"
                                   "addi t1, zero, 2 \n"
                                   "addi t2, zero, 3 \n"
                                   "csrrw zero, mstatus, t0 \n"
                                   "csrrs zero, mtvec, t1 \n"
                                   "csrrw zero, mepc, t2 \n"
                                   "csrrc t2, mepc, zero \n"
                                   "csrrwi zero, sstatus, 4 \n"
                                   "csrrsi zero, stvec, 5 \n"
                                   "csrrwi zero, sepc, 6 \n"
                                   "csrrci zero, sepc, 0 \n";

        CPU cpu = rvHelper(code, "test_csrs1", 11);

        REQUIRE(cpu.getRegValueByName("mstatus") == 1);
        REQUIRE(cpu.getRegValueByName("mtvec") == 2);
        REQUIRE(cpu.getRegValueByName("mepc") == 3);
        REQUIRE(cpu.getRegValueByName("sstatus") == 0);
        REQUIRE(cpu.getRegValueByName("stvec") == 5);
        REQUIRE(cpu.getRegValueByName("sepc") == 6);
    }
}    // namespace rvemu
