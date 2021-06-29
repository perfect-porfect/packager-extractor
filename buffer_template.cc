#include "buffer_template.h"

template <class PacketSections>
Buffer<PacketSections>::Buffer(uint32_t size)
    : size_(size)
{
    write_index_ = 0;
    read_index_ = 0;
    buffer_.resize(size_);
}

template<class PacketSections>
void Buffer<PacketSections>::write(PacketSections data)
{
    buffer_[write_index_] = data;
    write_index_++;
}

template<class PacketSections>
PacketSections Buffer<PacketSections>::get_next_packet()
{

}
