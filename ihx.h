#pragma once

#include <cstdint>

bool read_record(FILE *fp,
                 uint8_t* buf, int buf_len,
                 uint16_t& adrs, int& len);
