#include "Csr.hpp"

#include "RVEmu.hpp"

#include <cassert>
#include <fmt/core.h>
#include <string>
#include <unordered_map>

namespace rvemu
{
    void CSRInterface::write(const AddrType dest, const RegisterSizeType what)
    {
        assert(checkLimit(dest));
        csrs_[dest] = what;
    }

    RegisterSizeType CSRInterface::read(AddrType where) const
    {
        assert(checkLimit(where));
        return csrs_[where];
    }

    /**
     * @brief Prints the current state of the control status registers.
     *
     * This function prints the current state of the control status registers in a
     * formatted manner. It prints the values of MSTATUS, MTVEC, MEPC, MCAUSE, SSTATUS,
     * STVEC, SEPC, and SCAUSE registers.
     */
    void CSRInterface::dumpCSRs() const
    {
        fmt::print("{:-^100}\n", "control status registers");
        fmt::print("{}\n{}\n",
                   fmt::format(
                       "mstatus = {:<#18x}  mtvec = {:<#18x}  mepc = {:<#18x}  mcause = {:<#18x}",
                       this->read(MSTATUS),
                       this->read(MTVEC),
                       this->read(MEPC),
                       this->read(MCAUSE)),
                   fmt::format(
                       "sstatus = {:<#18x}  stvec = {:<#18x}  sepc = {:<#18x}  scause = {:<#18x}",
                       this->read(SSTATUS),
                       this->read(STVEC),
                       this->read(SEPC),
                       this->read(SCAUSE)));
    }

    const std::unordered_map<std::string, std::size_t> CSRInterface::csrAddrs_ = {
      {"mhartid",    MHARTID   },
      {"mstatus",    MSTATUS   },
      {"mtvec",      MTVEC     },
      {"mepc",       MEPC      },
      {"mcause",     MCAUSE    },
      {"mtval",      MTVAL     },
      {"medeleg",    MEDELEG   },
      {"mscratch",   MSCRATCH  },
      {"MIP",        MIP       },
      {"mcounteren", MCOUNTEREN},
      {"sstatus",    SSTATUS   },
      {"stvec",      STVEC     },
      {"sepc",       SEPC      },
      {"scause",     SCAUSE    },
      {"stval",      STVAL     },
      {"sscratch",   SSCRATCH  },
      {"SIP",        SIP       },
      {"SATP",       SATP      },
    };
}    // namespace rvemu
