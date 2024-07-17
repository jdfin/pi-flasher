#include <cstdint>
#include <cstdio>
#include "spi_dev.h"
#include "mcp23x17_spi.h"


Mcp23x17Spi::Mcp23x17Spi(SpiDev& spi_dev, uint8_t spi_adr) :
    Mcp23x17Bus(),
    _spi_dev(spi_dev),
    _spi_adr(spi_adr)
{
}


Mcp23x17Spi::~Mcp23x17Spi()
{
}


// write register
// always succeeds or throws
void Mcp23x17Spi::write_reg(uint8_t reg, uint16_t val)
{
    // val bits 0..7 are port A and go to reg;
    // bits 8..15 are port B and go to reg + 1

    //printf("write_reg(%d): reg=0x%02x val=0x%04x\n", _spi_adr, reg, val);

    // don't assume an endian
    uint8_t buf[4] = {
        uint8_t(_spi_adr << 1),
        reg,
        uint8_t(val),       // bits 0..7
        uint8_t(val >> 8)   // bits 8..15
    };

    _spi_dev.xfer(buf, buf, 4);

    _write_count++;
}


// read register
// always succeeds or throws
void Mcp23x17Spi::read_reg(uint8_t reg, uint16_t& val)
{
    // val bits 0..7 are port A and come from reg;
    // bits 8..15 are port B and come from reg + 1

    uint8_t buf[4] = {
        uint8_t((_spi_adr << 1) | 1),
        reg,
        0,                  // will get bits 0..7
        0                   // will get bits 8..15
    };

    _spi_dev.xfer(buf, buf, 4);

    // don't assume an endian
    val = (uint16_t(buf[3]) << 8) | uint16_t(buf[2]);

    //printf("read_reg(%d): reg=0x%02x val=0x%04x\n", _spi_adr, reg, val);

    _read_count++;
}
