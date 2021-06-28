#pragma once

#include <mutex>
#include <memory>
#include <memory.h>

#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include "buffer_base.h"

class FastBuffer : public AbstractBuffer
{

public:
    FastBuffer(uint32_t size = 2 * 1024 * 1024);
    char get_next_byte();

    BufferError read(uint8_t *data, const uint32_t len, const uint32_t timeout);
    BufferError write(const uint8_t *data, const uint32_t len);


    std::string get_next_bytes(const uint bytes);
    void receive_data(const char *buffer, const size_t size);
    void stop_waiting_for_data();
    uint8_t read_next_byte();
    ~FastBuffer();

private:
    char *input_buffer_;
    uint32_t volatile buffer_write_index_;
    uint32_t volatile buffer_read_index_;
    bool finish_waiting_;
    uint32_t buffer_size_;
};
