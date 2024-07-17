#pragma once

#include <cstdint>
#include "mcp23x17.h"

class Mcp23x17Bus;


class Mcp23x17
{

    public:

        Mcp23x17(Mcp23x17Bus& bus);

        ~Mcp23x17();

        // configure all pins
        // ins: 1=input, 0=output
        // pulls: 1=pullup enabled (for input pins)
        // vals: initial values for output pins
        void config(uint16_t ins, uint16_t pulls, uint16_t vals);

        void set_bits(uint16_t bits, uint16_t vals);

        void get_bits(uint16_t bits, uint16_t& vals);

        void set_dirs(uint16_t bits, uint16_t dirs);

        void get_dirs(uint16_t bits, uint16_t& dirs);

        void show();

    private:

        Mcp23x17Bus& _bus;

        bool _ready;

        // this should always match chip's olat, so we don't have
        // to read it each time we want to change a bit's value
        uint16_t _olat;

        // this should always match chip's iodir, so we don't have
        // to read it each time we want to change a bit's direction
        uint16_t _iodir;

        // BANK is always zero
        enum Reg {
            IODIR = 0x00,
            IPOL = 0x02,
            GPINTEN = 0x04,
            DEFVAL = 0x06,
            INTCON = 0x08,
            IOCON = 0x0a,
            GPPU = 0x0c,
            INTF = 0x0e,
            INTCAP = 0x10,
            GPIO = 0x12,
            OLAT = 0x14
        };

}; // class Mcp23x17
