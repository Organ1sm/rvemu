#include "CSR.hpp"
#include "RVEmu.h"
#include <cstddef>
#include <fmt/core.h>
#include <fmt/format.h>


namespace rvemu
{
    /**
     * @brief Constructor for the Csr class.
     *
     * This constructor initializes all CSR (Control Status Registers) to 0.
     */
    Csr::Csr()
    {
        csrs_.fill(0);    // 初始化所有CSR为0
    }

    /**
     * @brief Prints the current state of the control status registers.
     *
     * This function prints the current state of the control status registers in a
     * formatted manner. It prints the values of MSTATUS, MTVEC, MEPC, MCAUSE, SSTATUS,
     * STVEC, SEPC, and SCAUSE registers.
     */
    void Csr::dumpCSRs() const
    {
        fmt::print("{:-^100}\n", "control status registers");
        fmt::print(
            "{}\n{}\n",
            fmt::format(
                "mstatus = {:<#18x}  mtvec = {:<#18x}  mepc = {:<#18x}  mcause = {:<#18x}",
                this->load(MSTATUS),
                this->load(MTVEC),
                this->load(MEPC),
                this->load(MCAUSE)),
            fmt::format(
                "sstatus = {:<#18x}  stvec = {:<#18x}  sepc = {:<#18x}  scause = {:<#18x}",
                this->load(SSTATUS),
                this->load(STVEC),
                this->load(SEPC),
                this->load(SCAUSE)));
    }

    /**
     * @brief Loads the value of a specific CSR.
     *
     * This function loads the value of a specific CSR from the array of CSRs.
     * It returns the value of the CSR at the specified address.
     *
     * @param addr The address of the CSR to load.
     * @return The value of the CSR at the specified address.
     */
    uint64_t Csr::load(std::size_t addr) const
    {
        switch (addr)
        {
            case SIE: return csrs_[MIE] & csrs_[MIDELEG];
            case SIP: return csrs_[MIP] & csrs_[MIDELEG];
            case SSTATUS: return csrs_[MSTATUS] & MASK_SSTATUS;
            default: return csrs_[addr];
        }
    }

    /**
     * @brief Stores a value into a specific CSR.
     *
     * This function stores a value into a specific CSR at the given address.
     * Depending on the address, the function performs different operations:
     * - If the address is SIE, it stores the value into the MIE register, taking into
     * account the MIDELEG register.
     * - If the address is SIP, it stores the value into the MIP register, taking into
     * account the MIDELEG register.
     * - If the address is SSTATUS, it stores the value into the MSTATUS register, taking
     * into account the MASK_SSTATUS.
     * - For any other address, it directly stores the value into the CSR at the given
     * address.
     *
     * @param addr The address of the CSR to store the value into.
     * @param value The value to store into the CSR.
     */
    void Csr::store(std::size_t addr, uint64_t value)
    {
        switch (addr)
        {
            case SIE:
                csrs_[MIE] = (csrs_[MIE] & ~csrs_[MIDELEG]) | (value & csrs_[MIDELEG]);
                break;
            case SIP:
                csrs_[MIP] = (csrs_[MIE] & ~csrs_[MIDELEG]) | (value & csrs_[MIDELEG]);
                break;
            case SSTATUS:
                csrs_[MSTATUS] =
                    (csrs_[MSTATUS] & ~MASK_SSTATUS) | (value & MASK_SSTATUS);
                break;
            default: csrs_[addr] = value;
        }
    }

    /**
     * @brief Checks if a specific bit in the MEDELEG register is set.
     *
     * This function checks if the bit at the position specified by 'val' in the MEDELEG
     * register is set. The MEDELEG register is used to delegate some exceptions from
     * M-mode to S-mode. If the bit is set, it means that the corresponding exception is
     * delegated.
     *
     * @param val The position of the bit to check in the MEDELEG register.
     * @return true if the bit at position 'val' is set, false otherwise.
     */
    bool Csr::isMedelegated(uint64_t val) const
    {
        return (csrs_[MEDELEG] >> val & 1) == 1;
    }

    /**
     * @brief Checks if a specific bit in the MIDELEG register is set.
     *
     * This function checks if the bit at the position specified by 'val' in the MIDELEG
     * register is set. The MIDELEG register is used to delegate some interrupts from
     * M-mode to S-mode. If the bit is set, it means that the corresponding interrupt is
     * delegated.
     *
     * @param val The position of the bit to check in the MIDELEG register.
     * @return true if the bit at position 'val' is set, false otherwise.
     */
    bool Csr::isMidelegated(uint64_t cause) const
    {
        return (csrs_[MIDELEG] >> cause & 1) == 1;
    }
}    // namespace rvemu
