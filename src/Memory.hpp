#pragma once

#include "RVEmu.hpp"

#include <cstddef>
#include <iostream>
#include <vector>

namespace rvemu
{
    using MemoryType = std::vector<std::byte>;

    RegisterSizeType readFromMemory(MemoryType &, RegisterSizeType, AddrType, DataSizeType);
    void writeToMemory(MemoryType &, AddrType base, AddrType, RegisterSizeType, DataSizeType);
    std::ostream &operator<< (std::ostream &os, std::byte b);

    class DRAM
    {
      public:
        DRAM() : dram_ {DRAM_SIZE, std::byte {0}} { }

        /// Writes data to a specified address in DRAM.
        /// @param addr The memory address to write to.
        /// @param value The data to write.
        /// @param size The size of the data to write.
        void write(AddrType addr, RegisterSizeType value, DataSizeType size);

        /// Reads data from a specified address in DRAM.
        /// @param addr The memory address to read from.
        /// @param size The size of the data to read.
        /// @return The data read from the memory.
        RegisterSizeType read(AddrType addr, DataSizeType size);

      private:
        MemoryType dram_;    /// The underlying storage for DRAM.
    };

    class SystemInterface
    {
      public:
        /// Constructs a system interface and initializes the DRAM.
        /// @param codePath The file path to the binary code to load into memory.
        SystemInterface(const std::string &codePath);

        /// Reads data from the system memory.
        /// @param addr The memory address to read from.
        /// @param size The size of the data to read.
        /// @return The data read from the memory.
        RegisterSizeType readData(AddrType addr, DataSizeType size);

        /// Writes data to the system memory.
        /// @param addr The memory address to write to.
        /// @param value The data to write.
        /// @param size The size of the data to write.
        void writeData(AddrType addr, RegisterSizeType value, DataSizeType size);

        /// Retrieves the last executed instruction address.
        /// @return The address of the last executed instruction.
        RegisterSizeType getLastInstr() { return lastInst_; }

      private:
        /// Loads binary code into memory from a specified file path.
        /// @param codePath The file path to the binary code to load.
        void loadCode(const std::string &codePath);

        /// Checks if the given memory address is within the valid address space.
        /// @param addr The memory address to check.
        /// @return True if the address is within the limit; false otherwise.
        bool checkLimit(AddrType addr);

        /// Validates if writing to the given memory address is permitted.
        /// @param addr The memory address to validate.
        /// @return True if writing is valid; false otherwise.
        bool validWrite(AddrType addr);

      private:
        DRAM memory_;          /// The DRAM instance used by the system interface.
        AddrType lastInst_;    /// The address of the last executed instruction.
    };
}    // namespace rvemu
