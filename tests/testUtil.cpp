#include "testUtil.hpp"

#include "../Log.hpp"
#include "../src/Emulator.hpp"

#include <cstdlib>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fstream>
#include <stdexcept>

namespace rvemu
{
    void generateRVAssemly(const std::string &csrc)
    {
        std::string command = "clang -S" + csrc + " -o ";

        int result = std::system(command.c_str());
        if (result != 0)
            throw std::runtime_error("Failed to generate RV assembly. Command: " + command);
    }

    void generateRVObj(const std::string &assembly)
    {
        std::size_t dotPos = assembly.find_last_of(".");
        std::string baseName =
            (dotPos == std::string::npos) ? assembly : assembly.substr(0, dotPos);

        std::string command = "clang "
                              "-Wl,-Ttext=0x0 "
                              "-nostdlib "
                              "--target=riscv64 "
                              "-march=rv64g "
                              "-mno-relax"
                              " -o "
                              + baseName + " " + assembly;

        int result = std::system(command.c_str());
        if (result != 0)
            fmt::print(std::cerr, "Failed to generate RV object from assembly: {}\n", assembly);
    }

    void generateRVBinary(const std::string &obj)
    {
        std::string command = "llvm-objcopy -O binary " + obj + " " + obj + ".bin";
        int result          = std::system(command.c_str());

        if (result != 0)
            fmt::print(std::cerr, "Failed to generate RV binary from object {}\n", obj);
    }

    const CPU &rvHelper(const std::string &code, const std::string &testname, std::size_t nClock)
    {
        std::string filename = testname + ".s";
        std::ofstream file(filename);
        if (!file.is_open())
            LOG(ERROR, "Failed to create assembly file.");

        file << code;
        file.close();

        generateRVObj(filename.c_str());
        generateRVBinary(testname.c_str());

        std::string binFile = testname + ".bin";
        rvemu::Emulator rvEmulator(binFile);
        rvEmulator.runEmulator();
        fmt::print(fg(colors[DEBUG]), "{:=^100}\n", "Debug");

        return rvEmulator.getCPU();
    }
}    // namespace rvemu
