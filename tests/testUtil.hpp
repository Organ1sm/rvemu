#pragma once

#include <string>
#include "../CPU.hpp"

namespace rvemu
{
    const std::string start = ".global _start \n _start: \n";

    void generateRVAssembly(const std::string &csrc);
    void generateRVObj(const std::string &assembly);
    void generateRVBinary(const std::string &obj);
    CPU rvHelper(const std::string &code,
                 const std::string &testname,
                 std::size_t nclock);

}    // namespace rvemu
