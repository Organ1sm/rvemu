#pragma once

#include "../src/Cpu.hpp"

#include <string>

namespace rvemu
{
    const std::string start = ".global _start \n _start: \n";

    void generateRVAssembly(const std::string &csrc);
    void generateRVObj(const std::string &assembly);
    void generateRVBinary(const std::string &obj);
    const CPU &rvHelper(const std::string &code, const std::string &testname, std::size_t nclock);

}    // namespace rvemu
