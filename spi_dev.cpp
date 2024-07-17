#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "spi_dev.h"


SpiDev::SpiDev(const char *spi_dev, uint32_t freq) :
    _fd(open(spi_dev, O_RDWR))
{
    if (_fd < 0)
        throw std::runtime_error("SpiDev::SpiDev: _fd < 0");

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    if (ioctl(_fd, SPI_IOC_WR_MODE, &mode) < 0 ||
        ioctl(_fd, SPI_IOC_RD_MODE, &mode) < 0 ||
        ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 ||
        ioctl(_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0 ||
        ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &freq) < 0 ||
        ioctl(_fd, SPI_IOC_RD_MAX_SPEED_HZ, &freq) < 0)
        throw std::runtime_error("SpiDev::SpiDev: ioctl < 0");
}


SpiDev::~SpiDev()
{
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}


// do a spi transfer
// always succeeds or throws an error
void SpiDev::xfer(uint8_t *tx_buf, uint8_t *rx_buf, int xfer_len)
{
    if (xfer_len == 0)
        return;

    if (tx_buf == nullptr || rx_buf == nullptr)
        throw std::invalid_argument("SpiDev::xfer: tx_buf == nullptr || rx_buf == nullptr");

    struct spi_ioc_transfer spi;
    memset(&spi, 0, sizeof(spi));
    spi.tx_buf = __u64(tx_buf);
    spi.rx_buf = __u64(rx_buf);
    spi.len = xfer_len;
    if (ioctl(_fd, SPI_IOC_MESSAGE(1), &spi) < 0)
        throw std::runtime_error("SpiDev::xfer: ioctl");
}
