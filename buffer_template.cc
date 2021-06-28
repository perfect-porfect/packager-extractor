#include "buffer_template.h"

template <class Type>
Buffer<Type>::Buffer(uint32_t size)
    : size_(size)
{
    write_index_ = 0;
    read_index_ = 0;
    buffer_.resize(size_);
}

template<class Type>
void Buffer<Type>::write(Type data)
{
    buffer_[write_index_] = data;
}

template<class Type>
Type Buffer<Type>::get_next_packet()
{

}
