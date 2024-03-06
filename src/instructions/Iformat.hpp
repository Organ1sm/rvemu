#pragma once

#include "InstFormat.hpp"

namespace rvemu
{
    class I : public InstructionFormat
    {
      public:
        void readRegister(const Registers &) override;
        void writeBack(Registers &) override;

      protected:
        I(InstSizeType is, AddrType pc)
          : InstructionFormat(is, pc), rsIdx_(takeRegS()), rdIdx_(takeRegD()),
            offset_(takeOffset()), func3_(takeFunc3())
        { }

      private:
        std::size_t takeRegS();
        std::size_t takeRegD();
        RegisterSizeType takeOffset();
        u8 takeFunc3();

      protected:
        std::size_t rsIdx_;
        std::size_t rdIdx_;
        RegisterSizeType offset_;
        u8 func3_;

        RegisterSizeType rs_;
        RegisterSizeType rd_;
    };

    class ImmOp : public I
    {
      private:
        // There are two duplicated func3 for which we need additional
        // info: the two last bits of the immediate.
        enum class Func3Type : u8 {
            Addi       = 0b000,
            Slli       = 0b001,
            Slti       = 0b010,
            Sltiu      = 0b011,
            Xori       = 0b100,
            SrliOrSrai = 0b101,
            Ori        = 0b110,
            Andi       = 0b111,
        };

      public:
        ImmOp(const InstSizeType is, const AddrType pc) : I(is, pc), func3_ {I::func3_} { }

        void execution() override;

      private:
        void addi();
        void slti();
        void sltiu();
        void xori();
        void ori();
        void andi();
        void slli();
        void srli();
        void srai();
        void printInstruction(const std::string &is_name, const std::string &op);
        Func3Type func3_;
    };

    class Jris : public I
    {
      public:
        Jris(const InstSizeType is, const AddrType pc) : I(is, pc) { }

        void execution() override;
        AddrType moveNextInst() override;
        ~Jris() = default;

      private:
        RegisterSizeType m_reg;
    };

    class Load : public I
    {
      public:
        Load(const InstSizeType is, const AddrType pc) : I(is, pc) { }

        void execution() override;
        void accessMemory(SystemInterface &) override;

      private:
        AddrType m_ad_to_read;
    };
}    // namespace rvemu
