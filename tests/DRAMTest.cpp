#include "catch2/catch_test_macros.hpp"

namespace rvemu
{
    // class DramTest
    // {
    //   protected:
    //     std::vector<uint8_t> code {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    //     DRAM dram {code};
    // };
    //
    // TEST_CASE_METHOD(DramTest, "load test")
    // {
    //     auto value = dram.load(DRAM_BASE, 64);
    //     REQUIRE(value.has_value() == true);
    //     REQUIRE(value.value() == 0x0807'0605'0403'0201);
    // }
    //
    // TEST_CASE_METHOD(DramTest, "store test")
    // {
    //     uint64_t storeValue = 0x0102'0304'0506'0708;
    //     REQUIRE(dram.store(DRAM_BASE, 64, storeValue) == true);
    //
    //     auto loadValue = dram.load(DRAM_BASE, 64);
    //     REQUIRE(loadValue.has_value() == true);
    //     REQUIRE(loadValue.value() == storeValue);
    // }
    //
    // TEST_CASE_METHOD(DramTest, "Invalid load test")
    // {
    //     auto loadValue = dram.load(DRAM_BASE, 10);
    //     REQUIRE(loadValue.has_value() == false);
    // }
    //
    // TEST_CASE_METHOD(DramTest, "Invalid store test")
    // {
    //     REQUIRE(dram.store(DRAM_BASE, 10, 0x01) == false);
    // }
}    // namespace rvemu
