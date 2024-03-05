#include "Emulator.hpp"

#include <cstring>
#include <iostream>

constexpr size_t max_len = 100;

int main(int argc, char **argv)
{
    int fileIdx = 1;
    if (argc < 2)
    {
        std::cerr << "Error: no file provided\n";
        return EXIT_FAILURE;
    }

    if (std::strlen(argv[fileIdx]) > max_len)
    {
        std::cerr << "Error: file length exceeds the limit: " << max_len << std::endl;
        return EXIT_FAILURE;
    }

    std::string bin_file {argv[fileIdx]};
    std::cout << "File provided: " << bin_file << std::endl;

    rvemu::Emulator riscv_emulator(bin_file);

    riscv_emulator.runEmulator();

    return EXIT_SUCCESS;
}
