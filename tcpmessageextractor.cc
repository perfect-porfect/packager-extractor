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
}

std::shared_ptr<AbstractSerializableMessage> TCPMessageExtractor::find_message()
{
    if (has_header_) {
        find_header();
    }
    if (has_cmd_) {

    }
}

void TCPMessageExtractor::find_header()
{
    int header_index = 0;
    while(1) {
        if (header_index == header_.len)
            break;
        if (header_.content[header_index] ==  buffer_->get_next_byte())
            header_index++;
        else
            header_index = 0;
    }
}

void TCPMessageExtractor::find_cmd()
{

}

void TCPMessageExtractor::find_len()
{

}
