#include "fast_buffer.h"

#include <thread>

FastBuffer::FastBuffer(uint32_t size)
    : buffer_size_(size)
{
    finish_waiting_ = true;
    buffer_read_index_ = 0;
    buffer_write_index_ = 0;
    input_buffer_ = new char[buffer_size_];
}

char FastBuffer::get_next_byte()
{
    while (buffer_read_index_ == buffer_write_index_) {
        if (!finish_waiting_)
           return '\0';
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    char data = input_buffer_[buffer_read_index_];
    buffer_read_index_++;
    if (buffer_read_index_ == buffer_size_)
        buffer_read_index_ = 0;
    return data;
}

BufferError FastBuffer::read(uint8_t *data, const uint32_t len, const uint32_t timeout)
{

}

BufferError FastBuffer::write(const uint8_t *data, const uint32_t len)
{

}

std::string FastBuffer::get_next_bytes(const uint bytes)
{
    std::string all_data;
    all_data.resize(bytes);
    for (uint bytes_no = 0; bytes_no < bytes; bytes_no++) {
        char data = get_next_byte();
        all_data[bytes_no] = data;
    }
    return all_data;
}

void FastBuffer::receive_data(const char *buffer, const size_t size)
{
    if (buffer_write_index_ + size <= buffer_size_ ) {
        std::copy(buffer, buffer + size, &input_buffer_[buffer_write_index_]);
        buffer_write_index_ += size;
    } else {
        uint can_copy_size = buffer_size_ - buffer_write_index_;
        std::copy(buffer, buffer + can_copy_size, &input_buffer_[buffer_write_index_]);
        buffer_write_index_ = 0;
        uint remain = uint(size) - can_copy_size;
        std::copy(buffer + can_copy_size, buffer + remain + can_copy_size, &input_buffer_[buffer_write_index_]);
        buffer_write_index_ += remain;
    }
}

void FastBuffer::stop_waiting_for_data()
{
    finish_waiting_ = false;
}

FastBuffer::~FastBuffer()
{
    delete input_buffer_;
}


