#include <iostream>
#include <fstream>
#include "CPU.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage:\n"
                  << "- ./program_name <filename>\n";
        return 0;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<uint8_t> code(std::istreambuf_iterator<char>(file), {});
    CPU cpu(code);

    while (cpu.pc < cpu.dram.size() * sizeof(cpu.dram[0]))
    {
        uint32_t inst = cpu.fetch();
        cpu.execute(inst);
        cpu.pc += 4;
    }

    cpu.dumpRegisters();
    return 0;
}
