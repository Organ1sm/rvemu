#include "catch2/catch_test_macros.hpp"
#include "../CSR.hpp"


namespace rvemu
{
    class CsrTest
    {
      protected:
        Csr csr;
    };

    TEST_CASE_METHOD(CsrTest, "csr load test")
    {
        csr.store(MSTATUS, 0x123456789abcdef0);
        auto value = csr.load(MSTATUS);
        REQUIRE(value == 0x123456789abcdef0);
    }

    TEST_CASE_METHOD(CsrTest, "csr store test")
    {
        csr.store(MSTATUS, 0x123456789abcdef0);
        auto value = csr.load(MSTATUS);
        REQUIRE(value == 0x123456789abcdef0);
    }

    TEST_CASE_METHOD(CsrTest, "csr Medelegated test")
    {
        csr.store(MEDELEG, 0x1);
        REQUIRE(csr.isMedelegated(0) == true);
    }

    TEST_CASE_METHOD(CsrTest, "csr Midelegated test")
    {
        csr.store(MIDELEG, 0x1);
        REQUIRE(csr.isMidelegated(0) == true);
    }
}    // namespace rvemu
