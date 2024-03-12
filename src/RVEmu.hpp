#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace rvemu
{
    constexpr std::size_t DRAM_BASE = 0x8000'0000;

    // define the size of DRAM => 128MB
    constexpr std::size_t DRAM_SIZE = 1024 * 1024 * 128;

    constexpr std::size_t DRAM_END = DRAM_BASE + DRAM_SIZE - 1;

    constexpr uint16_t NUM_CSRS = 4096;

    constexpr uint8_t RegistersNumber = 32;

    constexpr uint8_t LAST_OPCODE_DIGIT = 6;
    constexpr uint8_t OPCODE_LEN        = 7;

    // RISC-V privilege mode
    using Mode = uint64_t;
    /// User mode
    constexpr Mode User = 0b00;
    /// Supervisor mode
    constexpr Mode Supervisor = 0b01;
    /// Machine mode
    constexpr Mode Machine = 0b11;

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using i64 = int64_t;

    using InstSizeType        = uint32_t;
    using AddrType            = uint64_t;
    using RegisterSizeType    = uint64_t;
    using CSRRegisterSizeType = uint64_t;
    using RegType             = std::array<RegisterSizeType, RegistersNumber>;

    enum RegisterIndex : std::size_t { Zero = 0, RA = 1, SP = 2 };

    enum DataSizeType { Byte = 1, HalfWord = 2, Word = 4, DoubleWord = 8 };

    // Machine-Level CSR
    /// HardWare thread ID.
    constexpr size_t MHARTID = 0xf14;
    /// Machine status register.
    constexpr size_t MSTATUS = 0x300;
    /// Machine exception delefation register.
    constexpr size_t MEDELEG = 0x302;
    /// Machine interrupt delefation register.
    constexpr size_t MIDELEG = 0x303;
    /// Machine interrupt-enable register.
    constexpr size_t MIE = 0x304;
    /// Machine trap-handler base address.
    constexpr size_t MTVEC = 0x305;
    /// Machine counter enable.
    constexpr size_t MCOUNTEREN = 0x306;
    /// Scratch register for machine trap handlers.
    constexpr size_t MSCRATCH = 0x340;
    /// Machine exception program counter.
    constexpr size_t MEPC = 0x341;
    /// Machine trap cause
    constexpr size_t MCAUSE = 0x342;
    /// Machine bad address or instruction.
    constexpr size_t MTVAL = 0x343;
    /// Machine interrupt pending.
    constexpr size_t MIP = 0x344;

    // Supervisor-level CSRs.
    /// Supervisor status register.
    constexpr size_t SSTATUS = 0x100;
    /// Supervisor interrupt-enable register.
    constexpr size_t SIE = 0x104;
    /// Supervisor trap handler base address.
    constexpr size_t STVEC = 0x105;
    /// Scratch register for supervisor trap handlers.
    constexpr size_t SSCRATCH = 0x140;
    /// Supervisor exception program counter.
    constexpr size_t SEPC = 0x141;
    /// Supervisor trap cause.
    constexpr size_t SCAUSE = 0x142;
    /// Supervisor bad address or instruction.
    constexpr size_t STVAL = 0x143;
    /// Supervisor interrupt pending.
    constexpr size_t SIP = 0x144;
    /// Supervisor address translation and protection.
    constexpr size_t SATP = 0x180;

    // mstatus and sstatus field mask
    constexpr uint64_t MASK_SIE     = 1 << 1;
    constexpr uint64_t MASK_MIE     = 1 << 3;
    constexpr uint64_t MASK_SPIE    = 1 << 5;
    constexpr uint64_t MASK_UBE     = 1 << 6;
    constexpr uint64_t MASK_MPIE    = 1 << 7;
    constexpr uint64_t MASK_SPP     = 1 << 8;
    constexpr uint64_t MASK_VS      = 0b11 << 9;
    constexpr uint64_t MASK_MPP     = 0b11 << 11;
    constexpr uint64_t MASK_FS      = 0b11 << 13;
    constexpr uint64_t MASK_XS      = 0b11 << 15;
    constexpr uint64_t MASK_MPRV    = 1 << 17;
    constexpr uint64_t MASK_SUM     = 1 << 18;
    constexpr uint64_t MASK_MXR     = 1 << 19;
    constexpr uint64_t MASK_TVM     = 1 << 20;
    constexpr uint64_t MASK_TW      = 1 << 21;
    constexpr uint64_t MASK_TSR     = 1 << 22;
    constexpr uint64_t MASK_UXL     = 0b11ULL << 32;
    constexpr uint64_t MASK_SXL     = 0b11ULL << 34;
    constexpr uint64_t MASK_SBE     = 1ULL << 36;
    constexpr uint64_t MASK_MBE     = 1ULL << 37;
    constexpr uint64_t MASK_SD      = 1ULL << 63;
    constexpr uint64_t MASK_SSTATUS = MASK_SIE | MASK_SPIE | MASK_UBE | MASK_SPP | MASK_FS
                                      | MASK_XS | MASK_SUM | MASK_MXR | MASK_UXL | MASK_SD;

    // MIP / SIP field mask
    constexpr uint64_t MASK_SSIP = 1 << 1;
    constexpr uint64_t MASK_MSIP = 1 << 3;
    constexpr uint64_t MASK_STIP = 1 << 5;
    constexpr uint64_t MASK_MTIP = 1 << 7;
    constexpr uint64_t MASK_SEIP = 1 << 9;
    constexpr uint64_t MASK_MEIP = 1 << 11;

}    // namespace rvemu
