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
        //                 1=in    1=pu    vals
        mcp23x17[m].config(0xffff, 0x0000, 0x0000);
    }

    for (int m = 0; m < 2; m++) {
        printf("mcp23x17[%d]:\n", m);
        mcp23x17[m].show();
    }

    for (int m = 0; m < 2; m++) {
        printf("mcp23x17[%d]: ", m);
        fflush(NULL);
        const int ipol_reg = 2;
        for (int i = 1; i <= 0x10000; i = i << 1) {
            bus_dev[m].write_reg(ipol_reg, uint16_t(i));
            uint16_t r;
            bus_dev[m].read_reg(ipol_reg, r);
            if (r != uint16_t(i))
                printf("verify failed at m=%d i=0x%04x: r=0x%04x\n", m, i, r);
        }
        printf("ok\n");
    }

    return 0;
}
