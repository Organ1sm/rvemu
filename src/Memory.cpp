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
        std::ifstream input_file;

        input_file.open(file_name, std::ios::binary);
        if (input_file.is_open() == false)
        {
            std::cerr << "Invalid file name: " << file_name << "\n";
            abort();
        }

        AddrType instAddr = DRAM_BASE;
        unsigned char b;
        while (input_file.read(reinterpret_cast<char *>(&b), Byte))
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
    readFromMemory(MemoryType &mem, AddrType base, AddrType read_from, DataSizeType data_size)
    {
        RegisterSizeType data_to_take = 0;
        assert(read_from >= base);
        const size_t index = read_from - base;
        assert(index < mem.size());

        for (size_t i = 0; i < data_size; ++i)
        {
            data_to_take |= std::to_integer<decltype(data_to_take)>(mem[index + i]) << (8 * i);
        }
        return data_to_take;
    }

    bool SystemInterface::checkLimit(AddrType a)
    {
        size_t indx = a - DRAM_BASE;
        return (a >= DRAM_BASE) && indx < DRAM_BASE;
    }

    void writeToMemory(MemoryType &mem,
                       AddrType base,
                       AddrType where_to_write,
                       RegisterSizeType what_to_write,
                       DataSizeType size)
    {
#ifdef DEB_BIN_INS
        std::bitset<64> whs_b(what_to_write);
        std::cout << "what_to_store_binary = " << whs_b << std::endl;
#endif
        assert(where_to_write >= base);
        size_t indx = where_to_write - base;
        assert(indx < mem.size());

        for (size_t i = 0; i < size; ++i)
        {
            std::byte byte_to_store = std::byte(what_to_write >> (8 * i));
#ifdef DEB_BIN_INS
            std::cout << "byte_to_store = " << byte_to_store << "\n";
#endif
            mem[indx + i] = byte_to_store;
        }
    }

    RegisterSizeType SystemInterface::readData(AddrType read_from, DataSizeType sz)
    {
        assert(checkLimit(read_from));
        if (isAlign(read_from, sz) == false)
        {
            handleAlignmentEx();
        }
        return memory_.read(read_from, sz);
        throw("Invalid read location\n");
        abort();
    }

    bool SystemInterface::validWrite(AddrType write_to)
    {
        return lastInst_ <= write_to && write_to < (DRAM_BASE + DRAM_SIZE);
    }

    void
    SystemInterface::writeData(AddrType write_to, RegisterSizeType what_write, DataSizeType sz)
    {
        assert(checkLimit(write_to));
        if (isAlign(write_to, sz) == false)
        {
            handleAlignmentEx();
        }
        return memory_.write(write_to, what_write, sz);
        throw("Invalid write location\n");
        abort();
    }

    void DRAM::write(AddrType where_to_write, RegisterSizeType what_to_write, DataSizeType size)
    {
        writeToMemory(m_dram, DRAM_BASE, where_to_write, what_to_write, size);
    }

    RegisterSizeType DRAM::read(AddrType read_from, DataSizeType data_size)
    {
        return readFromMemory(m_dram, DRAM_BASE, read_from, data_size);
    }
}    // namespace rvemu
