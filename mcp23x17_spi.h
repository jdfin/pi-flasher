#pragma once

#include <cstdint>
#include "mcp23x17_bus.h"

class SpiDev;


class Mcp23x17Spi : public Mcp23x17Bus
{

    public:

        Mcp23x17Spi(SpiDev& spi_dev, uint8_t spi_adr);

        virtual ~Mcp23x17Spi();

        virtual void read_reg(uint8_t reg, uint16_t& val) override;

        virtual void write_reg(uint8_t reg, uint16_t val) override;

    private:

        SpiDev& _spi_dev;

        uint8_t _spi_adr;

}; // class Mcp23x17Spi

