#pragma once

#include <cstdint>


class SpiDev
{

    public:

        SpiDev(const char *spi_dev, uint32_t freq);

        ~SpiDev();

        void xfer(uint8_t *tx_buf, uint8_t *rx_buf, int xfer_len);

    private:

        int _fd;

}; // class SpiDev

