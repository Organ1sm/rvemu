#include "testUtil.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>


namespace rvemu
{
    TEST_CASE("RVTests", "Test Instrucitons")
    {
        SECTION("addi instruction with a postive immediate value")
        {
            std::string code = start + "addi x31, x0, 42 \n";
            CPU cpu          = rvHelper(code, "test_addi", 1);

            REQUIRE(cpu.regs[31] == 42);
        }

        SECTION("addi instruction with a negative immediate value")
        {
            std::string code = start + "addi x2, x0, -10 \n";
            CPU cpu          = rvHelper(code, "test_addi", 1);

            REQUIRE(cpu.regs[2] == -10);
        }

        SECTION("test slli instruction")
        {
            std::string code = start + "addi x2, x0, 5 \n"    // load 5 into x2
                                       "slli x1, x2, 3\n";    // x1 = x2 << 3

            CPU cpu = rvHelper(code, "test_slli", 2);
            REQUIRE(cpu.regs[1] == (5 << 3));
        }

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

        SECTION("test xori instruction")
        {
            std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                       "xori x1, x2, 7 \n";    // x1 =  x2 ^ 7

            CPU cpu = rvHelper(code, "test_xori", 2);
            REQUIRE(cpu.regs[1] == 8);
        }

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

        SECTION("test add instruction")
        {
            std::string code = start + "addi x2, x0, 10 \n"    // load 10 into x2
                                       "addi x3, x0, 20 \n"    // load 20 into x3
                                       "add x1, x2, x3 \n";    // x1 = x2 + x3

            CPU cpu = rvHelper(code, "test_add", 3);
            REQUIRE(cpu.regs[1] == 30);
        }

        SECTION("test ori instruction")
        {
            std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                       "ori x1, x2, 7 \n";     // x1 = x2 | 7

            CPU cpu = rvHelper(code, "test_ori", 3);
            REQUIRE(cpu.regs[1] == (15 | 7));
        }

        SECTION("test andi instruction")
        {
            std::string code = start + "addi x2, x0, 15 \n"    // load 15 into x2
                                       "andi x1, x2, 7 \n";     // x1 = x2 & 7

            CPU cpu = rvHelper(code, "test_ori", 3);
            REQUIRE(cpu.regs[1] == (15 & 7));
        }
    }
}    // namespace rvemu
