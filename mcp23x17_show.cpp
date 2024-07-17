#include <cstdint>
#include <cstdio>
#include "spi_dev.h"
#include "mcp23x17_spi.h"
#include "mcp23x17.h"
#include "hardware.h"


int main(int argc, char *argv[])
{
    SpiDev spi_dev(spi_dev_name, spi_dev_freq);

    Mcp23x17Spi bus_dev[] = {
        Mcp23x17Spi(spi_dev, spi_dev_adrs_0),
        Mcp23x17Spi(spi_dev, spi_dev_adrs_1)
    };

    Mcp23x17 mcp23x17[] = {
        Mcp23x17(bus_dev[0]),
        Mcp23x17(bus_dev[1])
    };

    for (int m = 0; m < 2; m++) {
        printf("mcp23x17[%d]:\n", m);
        mcp23x17[m].show();
    }

    return 0;
}
