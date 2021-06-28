#include "tcpmessageextractor.h"

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
    std::string data;
    bool is_crc_footer_ok = true;

    for (auto section : packet_sections_) {
        switch(section) {
        case Type::Header : {
            find_header();
            break;
        }
        case Type::CMD : {
            std::string cmd = find_cmd();
            auto msg = cmd_.factory->build_message(cmd.data());
            break;
        }
        case Type::Lenght : {
            int len = find_len();
            int include = len_.include;
            break;
        }
        case Type::Data : {

            break;
        }
        case Type::CRC : {

            break;
        }
        case Type::Footer : {

            break;
        }
        case Type::Other :{

            break;
        }
        }
    }
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

std::string TCPMessageExtractor::find_cmd()
{
    int header_index = 0;
    std::string cmd;
    cmd.resize(cmd_.len);
    for (int index = 0; index < cmd_.len; index++)
        cmd[index] = buffer_->read_next_byte();
    return cmd;
}

int TCPMessageExtractor::find_len()
{
    int len = 0;
    std::string len_data;
    len_data.resize(len_.len);
    for (int i = 0; i < len_.len; i++)
        len_data[i] = buffer_->read_next_byte();

    len = calc_len(len_data.data(), len_.len, len_.is_msb);
    return len;
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
           len =  int((unsigned char)(data[0]) << 24 | (unsigned char)(data[1]) << 16 |  (unsigned char)(data[2]) << 8 | (unsigned char)(data[3]));

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
