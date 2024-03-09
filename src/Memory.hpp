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
        DRAM() : m_dram {DRAM_SIZE, std::byte {0}} { }

        void write(AddrType, RegisterSizeType, DataSizeType);
        RegisterSizeType read(AddrType, DataSizeType);

      private:
        MemoryType m_dram;
    };

    class SystemInterface
    {
      public:
        SystemInterface(const std::string &);

        RegisterSizeType readData(AddrType, DataSizeType);
        void writeData(AddrType, RegisterSizeType, DataSizeType);

        RegisterSizeType getLastInstr() { return lastInst_; }
#ifdef DEB_BIN_INS
      private:
        void printCode();
#endif
      private:
        void loadCode(const std::string &);
        bool checkLimit(AddrType);
        bool validWrite(AddrType);

      private:
        DRAM memory_;
        AddrType lastInst_;
    };
}    // namespace rvemu
