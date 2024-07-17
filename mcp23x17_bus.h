#pragma once

#include <cstdint>


class Mcp23x17Bus
{

    public:

        Mcp23x17Bus() :
            _read_count(0),
            _write_count(0)
        {
        }

        virtual ~Mcp23x17Bus()
        {
        }

        virtual void read_reg(uint8_t reg, uint16_t& val) = 0;

        virtual void write_reg(uint8_t reg, uint16_t val) = 0;

        int read_count() const { return _read_count; }

        int write_count() const { return _write_count; }

    protected:

        int _read_count;
        int _write_count;

}; // class Mcp23x17Bus
