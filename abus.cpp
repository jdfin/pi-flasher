#include <cassert>
#include <cstdint>
#include "mcp23x17.h"
#include "abus.h"


ABus::ABus(Mcp23x17& mcp23x17_lo, Mcp23x17& mcp23x17_hi) :
           _mcp23x17_lo(mcp23x17_lo),
           _mcp23x17_hi(mcp23x17_hi),
           _adrs(0xffffffff)
{
    set(0);
}


ABus::~ABus()
{
}


void ABus::set(uint32_t adrs)
{
    assert((adrs & ~0x7ffff) == 0); // 19 bits max (512K x 8)

    uint16_t adrs_lo = adrs & 0xffff; // a[0-15] (16 bits)
    if (adrs_lo != (_adrs & 0xffff)) {
        _mcp23x17_lo.set_bits(0xffff, adrs_lo);
    }

    uint16_t adrs_hi = (adrs >> 16) & 0x7; // a[16-18] (3 bits)
    if (adrs_hi != ((_adrs >> 16) & 0x7)) {
        _mcp23x17_hi.set_bits(0x0700, adrs_hi << 8);
    }

    _adrs = adrs;
}
