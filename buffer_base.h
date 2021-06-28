#pragma once
#include <stdint.h>

enum BufferError : uint8_t {
    BUF_NOERROR = 0,
    BUF_OVERFLOW = 1,
    BUF_NODATA = 2,
    BUF_TIMEOUT = 3,
};

class AbstractBuffer
{
public:
    virtual BufferError read(uint8_t *data, const uint32_t len, const uint32_t timeout = 0) = 0;
    virtual uint8_t read_next_byte() = 0;
    virtual BufferError write(const uint8_t *data, const uint32_t len) = 0;
    virtual ~AbstractBuffer() {}
};
