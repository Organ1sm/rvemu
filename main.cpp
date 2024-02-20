#include <cstdint>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include "CPU.hpp"
#include "Exception.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage:\n"
                  << "- ./rvemu <filename>\n";
        return 0;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file)
    {
        fmt::print(std::cerr, "Cannot open file: {}\n", argv[1]);
        return 1;
    }

    std::vector<uint8_t> code(std::istreambuf_iterator<char>(file), {});
    CPU cpu(code);

    try
    {
        while (true)
        {
            uint32_t inst = cpu.fetch();
            auto newPC    = cpu.execute(inst);
            if (newPC.has_value())
                cpu.pc = newPC.value();
            else
                break;
        }
    }
    catch (const RVEmuException &e)
    {
        fmt::print(std::cerr, "RVEmuException main:  {}\n", e);
    }


    cpu.dumpRegisters();
    cpu.dumpPC();

    return 0;
}
