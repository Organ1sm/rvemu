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
    rvemu::CPU cpu(code);

    while (true)
    {
        auto inst = cpu.fetch();
        if (inst.has_value())
        {
            auto newPC = cpu.execute(inst.value());
            if (newPC.has_value())
                cpu.pc = newPC.value();
            else
                break;
        }
        else
        {
            break;
        }
    }

    cpu.dumpRegisters();
    cpu.dumpPC();

    return 0;
}
