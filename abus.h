#pragma once

#include <cstdint>

class Mcp23x17;


class ABus {

    public:

        ABus(Mcp23x17& mcp23x17_lo, Mcp23x17& mcp23x17_hi);
        ~ABus();

        void set(uint32_t adrs);

    private:

        Mcp23x17& _mcp23x17_lo; // bits 0-15
        Mcp23x17& _mcp23x17_hi; // bits 16-18

        uint32_t _adrs;

}; // class ABus
