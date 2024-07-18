#pragma once

#include <cstdint>


class Mcp23x17Bus
{

    public:

        Mcp23x17Bus()
        {
        }

        virtual ~Mcp23x17Bus()
        {
        }

        virtual void read_reg(uint8_t reg, uint16_t& val) = 0;

        virtual void write_reg(uint8_t reg, uint16_t val) = 0;

}; // class Mcp23x17Bus
