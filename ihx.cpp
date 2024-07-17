#include <stdexcept>
#include <cstdint>
#include <cstdio>
#include "ihx.h"


static uint8_t get_byte(char*& p)
{
    int msn;
    if (*p >= 'A' && *p <= 'F')
        msn = 10 + *p - 'A';
    else if (*p >= 'a' && *p <= 'f')
        msn = 10 + *p - 'a';
    else if (*p >= '0' && *p <= '9')
        msn = *p - '0';
    else
        throw std::runtime_error("ihx::get_byte: invalid character");

    p++;

    int lsn;
    if (*p >= 'A' && *p <= 'F')
        lsn = 10 + *p - 'A';
    else if (*p >= 'a' && *p <= 'f')
        lsn = 10 + *p - 'a';
    else if (*p >= '0' && *p <= '9')
        lsn = *p - '0';
    else
        throw std::runtime_error("ihx::get_byte: invalid character");

    p++;

    uint8_t byte = uint8_t(msn * 16 + lsn);

    //printf("byte=%02x\n", byte);

    return byte;
}


uint8_t read_line(FILE *fp,
                  uint8_t* buf, int buf_len,
                  uint16_t& adrs, int& len)
{
    char line[512];

    if (fgets(line, sizeof(line), fp) == NULL)
        throw std::runtime_error("ihx::read_line: no end record");

    char *p = line;

    // advance p until it points at the first character after the ':'
    while (*p != '\0' && *p != ':')
        p++;

    if (*p == '\0')
        throw std::runtime_error("ihx::read_line: invalid record");

    // p points at ':'
    p++;

    uint8_t cksum = 0;

    uint8_t data_len = get_byte(p);
    cksum += data_len;

    len = data_len;

    if (len > buf_len)
        throw std::runtime_error("ihx::read_file: buffer too small");

    uint8_t adrs_hi = get_byte(p);
    cksum += adrs_hi;

    uint8_t adrs_lo = get_byte(p);
    cksum += adrs_lo;

    adrs = (uint16_t(adrs_hi) << 8) | adrs_lo;

    uint8_t rec_type = get_byte(p);
    cksum += rec_type;

    if (rec_type == 0x00) {
        // read record to buffer
        while (data_len-- > 0) {
            uint8_t b = get_byte(p);
            *buf++ = b;
            cksum += b;
        }
        // get checksum byte
        uint8_t ck = get_byte(p);
        cksum += ck;
        if (cksum != 0)
            throw std::runtime_error("ihx::read_line: invalid checksum");
        if (*p != '\0' && *p != '\n')
            throw std::runtime_error("ihx::read_line: invalid record");
    }

    return rec_type;
}


void read_file(const char *filename,
               uint8_t* buf, int buf_len,
               uint16_t& adrs_min, uint16_t& adrs_max)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        throw std::runtime_error("ihx::read_file: error opening file");

    while (true) {
        char line[512];

        if (fgets(line, sizeof(line), fp) == NULL)
            throw std::runtime_error("ihx::read_file: no end record");

        char *p = line;

        // advance p until it points at the first character after the ':'
        while (*p != '\0' && *p != ':')
            p++;

        if (*p == '\0')
            throw std::runtime_error("ihx::read_file: invalid record");

        // p points at ':'
        p++;

        uint8_t cksum = 0;

        uint8_t data_len = get_byte(p);
        cksum += data_len;

        uint8_t adrs_hi = get_byte(p);
        cksum += adrs_hi;

        uint8_t adrs_lo = get_byte(p);
        cksum += adrs_lo;

        uint16_t adrs = (uint16_t(adrs_hi) << 8) | adrs_lo;

        uint8_t rec_type = get_byte(p);
        cksum += rec_type;

        if (rec_type == 0x00) {
            // read record to buffer
            while (data_len-- > 0) {
                if (adrs_min > adrs)
                    adrs_min = adrs;
                if (adrs_max < adrs)
                    adrs_max = adrs;
                if (adrs >= buf_len)
                    throw std::runtime_error("ihx::read_file: buffer too small");
                uint8_t b = get_byte(p);
                buf[adrs++] = b;
                cksum += b;
            }
            // get checksum byte
            uint8_t ck = get_byte(p);
            cksum += ck;
            if (cksum != 0)
                throw std::runtime_error("ihx::read_file: invalid checksum");
            if (*p != '\0' && *p != '\n')
                throw std::runtime_error("ihx::read_file: invalid record");
        } else if (rec_type == 0x01) {
            // end of file
            return;
        }

    } // while (true)

} // void read_file
