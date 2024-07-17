#include <iostream>
#include <iomanip>
#include <cstdint>
#include <unistd.h>
#include "mcp23x17_bus.h"
#include "mcp23x17.h"

using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::hex;
using std::dec;
using std::right;
using std::left;


Mcp23x17::Mcp23x17(Mcp23x17Bus& bus) :
    _bus(bus),
    _ready(false),
    _olat(0x0000),
    _iodir(0xffff)
{
}


Mcp23x17::~Mcp23x17()
{
    // all pins input
    _bus.write_reg(IODIR, 0xffff);
}


void Mcp23x17::config(uint16_t ins, uint16_t pulls, uint16_t vals)
{
    // use hardware address
    // there's only one 8-bit IOCON register,
    // so this is actually writing it twice (okay)
    _bus.write_reg(IOCON, 0x0808);

    // don't invert any inputs, everything else default for no interrupts
    _bus.write_reg(IPOL, 0);
    _bus.write_reg(GPINTEN, 0);
    _bus.write_reg(DEFVAL, 0);
    _bus.write_reg(INTCON, 0);

    // INTF is read-only
    // INTCAP is read-only

    // set output latches; output pins don't get driven until IODIR is
    // written below
    _bus.write_reg(GPIO, vals);
    _olat = vals;

    // OLAT is set by writing GPIO

    // this is where pins might change to outputs and become driven
    _bus.write_reg(IODIR, ins);
    _iodir = ins;

    // set this after IODIR so an errant bit in pulls can't pull
    // a pin the wrong way that is intended to be an output
    _bus.write_reg(GPPU, pulls);

    _ready = true;
}


void Mcp23x17::set_bits(uint16_t bits, uint16_t vals)
{
    if (!_ready)
        throw std::runtime_error("Mcp23x17::set_bits: !ready");

    _olat = (_olat & ~bits) | vals;

    _bus.write_reg(GPIO, _olat);
}


void Mcp23x17::get_bits(uint16_t bits, uint16_t& vals)
{
    if (!_ready)
        throw std::runtime_error("Mcp23x17::get_bits: !ready");

    _bus.read_reg(GPIO, vals);

    vals &= bits;
}


void Mcp23x17::set_dirs(uint16_t bits, uint16_t dirs)
{
    if (!_ready)
        throw std::runtime_error("Mcp23x17::set_dirs: !ready");

    _iodir = (_iodir & ~bits) | dirs;

    _bus.write_reg(IODIR, _iodir);
}


void Mcp23x17::get_dirs(uint16_t bits, uint16_t& dirs)
{
    if (!_ready)
        throw std::runtime_error("Mcp23x17::get_dirs: !ready");

    _bus.read_reg(IODIR, dirs);

    dirs &= bits;
}


void Mcp23x17::show()
{
    struct {
        const char *name;
        int adrs;
    } regs[] = {
        { "IODIR",   IODIR   },
        { "IPOL",    IPOL    },
        { "GPINTEN", GPINTEN },
        { "DEFVAL",  DEFVAL  },
        { "INTCON",  INTCON  },
        { "IOCON",   IOCON   },
        { "GPPU",    GPPU    },
        { "INTF",    INTF    },
        { "INTCAP",  INTCAP  },
        { "GPIO",    GPIO    },
        { "OLAT",    OLAT    },
    };
    const int regs_max = sizeof(regs) / sizeof(*regs);

    for (int r = 0; r < regs_max; r++) {
        uint16_t val;
        _bus.read_reg(regs[r].adrs, val);
        cout
            << setw(8) << setfill(' ') << left << regs[r].name
            << "0x" << hex << setw(4) << setfill('0') << right << val
            << endl;
    }
    cout << dec << setw(0);
}
