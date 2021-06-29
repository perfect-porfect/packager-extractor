#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>
#include <atomic>

template <class PacketSections>
class Buffer
{
public:
    Buffer(uint32_t size = 200);
    void write(PacketSections data);
    PacketSections get_next_packet();
private:
    uint32_t size_;
    std::atomic<int> write_index_;
    std::atomic<int> read_index_;
    std::vector<PacketSections> buffer_;
};

#endif // BUFFER_H
