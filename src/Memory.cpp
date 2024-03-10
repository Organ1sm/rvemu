#include "Memory.hpp"

#include "RVEmu.hpp"

#include <bitset>
#include <cassert>
#include <cstddef>
#include <fstream>

// LITTLE ENDIAN: the lew significant bit is stored in the lower address.
// Therefore 1100-0001 is stored as 0x0: 0001 0x1: 1100
namespace rvemu
{
    SystemInterface::SystemInterface(const std::string &fileName) : lastInst_ {0}
    {
        loadCode(fileName);
    }

    void SystemInterface::loadCode(const std::string &file_name)
    {
        std::ifstream inputFile;

        inputFile.open(file_name, std::ios::binary);
        if (inputFile.is_open() == false)
        {
            std::cerr << "Invalid file name: " << file_name << "\n";
            abort();
        }

        AddrType instAddr = DRAM_BASE;
        unsigned char b;
        while (inputFile.read(reinterpret_cast<char *>(&b), Byte))
        {
            memory_.write(instAddr, b, Byte);
            instAddr += Byte;
        }
        lastInst_ = instAddr;
    }

    std::ostream &operator<< (std::ostream &os, std::byte b)
    {
        return os << std::bitset<8>(std::to_integer<unsigned char>(b));
    }

    inline bool isAlign(AddrType ad, DataSizeType sz) { return ad % sz == 0; }

    inline void handleAlignmentEx() { std::cout << "Access to misaligned data\t"; }

    RegisterSizeType
    readFromMemory(MemoryType &mem, AddrType base, AddrType readFrom, DataSizeType dataSize)
    {
        RegisterSizeType dataToTake = 0;
        assert(readFrom >= base);
        const size_t index = readFrom - base;
        assert(index < mem.size());

        for (size_t i = 0; i < dataSize; ++i)
        {
            dataToTake |= std::to_integer<decltype(dataToTake)>(mem[index + i]) << (8 * i);
        }
        return dataToTake;
    }

    bool SystemInterface::checkLimit(AddrType a)
    {
        size_t index = a - DRAM_BASE;
        return (a >= DRAM_BASE) && index < DRAM_BASE;
    }

    void writeToMemory(MemoryType &mem,
                       AddrType base,
                       AddrType whereToWrite,
                       RegisterSizeType whatToWrite,
                       DataSizeType size)
    {
#ifdef DEB_BIN_INS
        std::bitset<64> whs_b(what_to_write);
        std::cout << "what_to_store_binary = " << whs_b << std::endl;
#endif
        assert(whereToWrite >= base);
        size_t indx = whereToWrite - base;
        assert(indx < mem.size());

        for (size_t i = 0; i < size; ++i)
        {
            std::byte byteToStore = std::byte(whatToWrite >> (8 * i));
#ifdef DEB_BIN_INS
            std::cout << "byte_to_store = " << byte_to_store << "\n";
#endif
            mem[indx + i] = byteToStore;
        }
    }

    RegisterSizeType SystemInterface::readData(AddrType readFrom, DataSizeType sz)
    {
        assert(checkLimit(readFrom));
        if (isAlign(readFrom, sz) == false)
        {
            handleAlignmentEx();
        }
        return memory_.read(readFrom, sz);
        throw("Invalid read location\n");
        abort();
    }

    bool SystemInterface::validWrite(AddrType writeTo)
    {
        return lastInst_ <= writeTo && writeTo < (DRAM_BASE + DRAM_SIZE);
    }

    void SystemInterface::writeData(AddrType writeTo, RegisterSizeType whatWrite, DataSizeType sz)
    {
        assert(checkLimit(writeTo));
        if (isAlign(writeTo, sz) == false)
        {
            handleAlignmentEx();
        }
        return memory_.write(writeTo, whatWrite, sz);
        throw("Invalid write location\n");
        abort();
    }

    void DRAM::write(AddrType whereToWrite, RegisterSizeType whatToWrite, DataSizeType size)
    {
        writeToMemory(dram_, DRAM_BASE, whereToWrite, whatToWrite, size);
    }

    RegisterSizeType DRAM::read(AddrType readFrom, DataSizeType dataSize)
    {
        return readFromMemory(dram_, DRAM_BASE, readFrom, dataSize);
    }
}    // namespace rvemu
