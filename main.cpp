#include <cstdint>
#include <fmt/ostream.h>
#include <iostream>
#include <fstream>
#include <optional>
#include "CPU.hpp"
#include "Log.hpp"

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
        if (!inst.has_value() || inst.value() == 0)
        {
            LOG(INFO, "END OF Program reached.");
            break;
        }


        auto newPC = cpu.execute(inst.value());
        if (!newPC.has_value())
        {
            LOG(ERROR, "Execution of instruction failed.");
            break;
        }

        cpu.pc = newPC.value();
    }

    cpu.dumpRegisters();
    cpu.dumpCSRs();
    cpu.dumpPC();

    return 0;
}
