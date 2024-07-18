#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include "spi_dev.h"
#include "mcp23x17_spi.h"
#include "mcp23x17.h"
#include "control.h"
#include "abus.h"
#include "dbus.h"
#include "flash.h"
#include "ihx.h"
#include "hardware.h"


int main(int argc, char *argv[])
{
    bool get_id = false;
    bool chip_erase = false;
    uint32_t dump_start = UINT32_MAX;
    uint32_t erase_start = UINT32_MAX;
    uint32_t op_len = UINT32_MAX;
    char *prog_file = NULL;

    int opt;
    char *endptr;
    while ((opt = getopt(argc, argv, "cid:e:l:p:")) != -1) {
        switch (opt) {
            case 'c':
                chip_erase = true;
                break;
            case 'd':
                dump_start = strtol(optarg, &endptr, 0);
                if (endptr == optarg || *endptr != '\0')
                    throw std::invalid_argument(optarg);
                break;
            case 'e':
                erase_start = strtol(optarg, &endptr, 0);
                if (endptr == optarg || *endptr != '\0')
                    throw std::invalid_argument(optarg);
                if ((erase_start & (Flash::sector_size - 1)) != 0)
                    throw std::invalid_argument(optarg);
                break;
            case 'i':
                get_id = true;
                break;
            case 'l':
                op_len = strtol(optarg, &endptr, 0);
                if (endptr == optarg || *endptr != '\0')
                    throw std::invalid_argument(optarg);
                break;
            case 'p':
                prog_file = strdup(optarg);
                if (prog_file == NULL)
                    throw std::bad_alloc();
                break;
        }
    }

    SpiDev spi_dev(spi_dev_name, spi_dev_freq);

    Mcp23x17Spi mcp23x17_spi_0(spi_dev, spi_dev_adrs_0);
    Mcp23x17Spi mcp23x17_spi_1(spi_dev, spi_dev_adrs_1);

    // constructors do not touch the device
    Mcp23x17 mcp23x17_0(mcp23x17_spi_0);
    Mcp23x17 mcp23x17_1(mcp23x17_spi_1);

    // a[0-15]
    mcp23x17_0.config(0x0000, 0x0000, 0x0000); // ins, pulls, vals (all outputs, 0)

    // (lsb) d[0-7] : a[16-18], x, x, we, oe, ce (msb)
    mcp23x17_1.config(0x00ff, 0x0000, 0x0000); // ins, pulls, vals (data in, adrs/ctrl out)

    Control ctrl(mcp23x17_1);
    DBus dbus(mcp23x17_1);
    ABus abus(mcp23x17_0, mcp23x17_1);

    Flash flash(ctrl, abus, dbus);

    if (get_id) {
        uint8_t mfg_id;
        uint8_t dev_id;
        flash.get_id(mfg_id, dev_id);
        printf("mfg_id=0x%02x dev_id=0x%02x\n", int(mfg_id), int(dev_id));
    }

    if (chip_erase) {
        flash.erase_chip();
    } else if (erase_start != UINT32_MAX && op_len != UINT32_MAX) {
        if ((op_len & (Flash::sector_size - 1)) != 0)
            throw std::invalid_argument("erase_length");
        while (op_len > 0) {
            flash.erase_sector(erase_start);
            erase_start += Flash::sector_size;
            op_len -= Flash::sector_size;
        }
    }

    if (prog_file != NULL) {
        printf("programming %s\n", prog_file);
        FILE *fp = fopen(prog_file, "r");
        if (fp == NULL)
            throw std::runtime_error("error opening file");
        const int buf_len = 256;
        uint8_t *buf = (uint8_t *)malloc(buf_len);
        if (buf == NULL)
            throw std::bad_alloc();

        while (true) {
            uint16_t adrs;
            int len;
            if (!read_record(fp, buf, buf_len, adrs, len))
                break;
            printf("adrs=%04x len=%02x:", adrs, len);
            for (int i = 0; i < len; i++)
                printf(" %02x", buf[i]);
            printf("\n");
            flash.program(adrs, buf, len);
        }
    }

    if (dump_start != UINT32_MAX && op_len != UINT32_MAX)
        flash.dump(dump_start, op_len);

    return 0;
}
