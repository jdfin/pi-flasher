#pragma once

#include <cstdint>

uint8_t read_line(FILE *fp,
                  uint8_t* buf, int buf_len,
                  uint16_t& adrs, int& len);

void read_file(const char *filename,
               uint8_t* buf, int buf_len,
               uint16_t& adrs_min, uint16_t& adrs_max);
