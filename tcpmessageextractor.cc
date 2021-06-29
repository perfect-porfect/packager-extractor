#include "tcpmessageextractor.h"
#include <iostream>

TCPMessageExtractor::TCPMessageExtractor(std::shared_ptr<AbstractRawExtractor> extractor, std::shared_ptr<AbstractBuffer> buffer)
    : extractor_(extractor), buffer_(buffer)
{
    has_header_ = extractor_->has_header();
    has_footer_ = extractor_->has_foofer();
    has_len_ = extractor_->has_len();
    has_cmd_ = extractor_->has_cmd();
    has_crc_ = extractor_->has_crc();

    header_ = extractor_->get_header();
    footer_ = extractor_->get_footer();
    len_ = extractor_->get_len();
    cmd_ = extractor_->get_cmd();
    crc_ = extractor_->get_crc();

    //    packet_sections_ = extractor_->get_sections();

}

std::shared_ptr<AbstractSerializableMessage> TCPMessageExtractor::find_message()
{
    std::shared_ptr<AbstractSerializableMessage> msg;
    uint8_t* data;
    bool is_crc_ok = true;
    bool is_footer_ok = true;
    bool has_len = false;
    int data_len;
    int include;
    for (auto section : packet_sections_) {
        switch(section) {
        case Type::Header : {
            find_header();
            break;
        }
        case Type::CMD : {
            std::string cmd = get_next_bytes(cmd_.len);
            auto msg = cmd_.factory->build_message(cmd.data());
            break;
        }
        case Type::Lenght : {
            std::string len_size = get_next_bytes(len_.len);
            data_len = calc_len(len_size.data(), len_.len, len_.is_msb);
            include = len_.include;
            has_len = true;
            break;
        }
        case Type::Data : {
            if (has_len) {
                data = new uint8_t[data_len];
            } else {
                data = new uint8_t[msg->get_serialize_size()];
            }

            auto buf_ret = buffer_->read(data, data_len);
            if (buf_ret != BufferError::BUF_NOERROR) {
                std::cout << "fucking buffer" << std::endl;
            }
            break;
        }
        case Type::CRC : {
            std::string crc_data = get_next_bytes(crc_.len);
            is_crc_ok = crc_.crc_checker->is_valid((char*)data, data_len, crc_data.data(), crc_data.size());
            break;
        }
        case Type::Footer : {
            is_footer_ok = can_find_footer();
            break;
        }
        case Type::Other :{

            break;
        }
        }
    }
    if (is_crc_ok && is_footer_ok)
        msg->serialize((char*)data, data_len);

    return msg;
}

void TCPMessageExtractor::find_header()
{
    int header_index = 0;
    while(1) {
        if (header_index == header_.len)
            break;
        if (header_.content[header_index] ==  buffer_->read_next_byte())
            header_index++;
        else
            header_index = 0;
    }
}


int TCPMessageExtractor::calc_len(const char * data, uint32_t size, bool is_msb)
{
    int len;
    switch (size) {
    case 1 : {
        len = (int)(*data);
        break;
    }
    case 2 : {
        if (is_msb)
            len = int((unsigned char)(data[0]) << 8 | (unsigned char)(data[1]));
        else
            len = int((unsigned char)(data[1]) << 8 | (unsigned char)(data[0]));
        break;
    }
    case 3 : {

    }
    case 4: {
        if (is_msb)
            len =  int((unsigned char)(data[0]) << 24 | (unsigned char)(data[1]) << 16 |  (unsigned char)(data[2]) << 8 | (unsigned char)(data[3]));
        else
            len =  int((unsigned char)(data[3]) << 24 | (unsigned char)(data[2]) << 16 |  (unsigned char)(data[1]) << 8 | (unsigned char)(data[0]));

    }
    case 5 : {

    }
    case 6 : {

    }
    case 7 : {

    }
    case 8 : {

    }
    }
}


bool TCPMessageExtractor::can_find_footer()
{
    std::string footer = get_next_bytes(footer_.len);
    for(int index = 0; index < footer_.len; index++) {
        if (footer[index] != footer_.content[index])
            return false;
    }
    return true;
}

std::string TCPMessageExtractor::get_next_bytes(uint32_t size)
{
    std::string data;
    data.resize(size);
    for (uint32_t i = 0; i < size; i++)
        data[i] = buffer_->read_next_byte();
    return data;
}
