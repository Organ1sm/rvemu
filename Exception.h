#pragma once

#include <cstdint>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <iostream>

class RVEmuException
{
  public:
    enum class Type {
        InstructionAddrMisaligned,
        InstructionAccessFault,
        IllegalInstruction,
        Breakpoint,
        LoadAccessMisaligned,
        LoadAccessFault,
        StoreAMOAddrMisaligned,
        StoreAMOAccessFault,
        EnvironmentCallFromUMode,
        EnvironmentCallFromSMode,
        EnvironmentCallFromMMode,
        InstructionPageFault,
        LoadPageFault,
        StoreAMOPageFault
    };

  private:
    Type type;
    uint64_t value;

  public:
    RVEmuException(Type type, uint64_t value) : type(type), value(value) {}

    friend std::ostream &operator<<(std::ostream &os, const RVEmuException &ex);

    uint64_t getValue() const;

    uint64_t getCode() const;

    bool isFatal() const;
};

template <>
struct fmt::formatter<RVEmuException> : ostream_formatter
{};
