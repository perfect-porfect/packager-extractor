#ifndef TCPMESSAGEEXTRACTOR_H
#define TCPMESSAGEEXTRACTOR_H

#include "abstract.h"

class TCPMessageExtractor {
public:
    TCPMessageExtractor(std::shared_ptr<AbstractRawExtractor> extractor, std::shared_ptr<AbstractBuffer> buffer);
    std::shared_ptr<AbstractSerializableMessage> find_message();
private:
    void find_header();
    std::string find_cmd();
    int find_len();
    int calc_len(const char *data, uint32_t size, bool is_msb);

    //Extractor
    std::shared_ptr<AbstractRawExtractor> extractor_;
    std::shared_ptr<AbstractBuffer> buffer_;
    std::shared_ptr<AbstractMessageFactory> message_factory_;
    bool has_extractor_;
    bool has_header_;
    bool has_footer_;
    bool has_len_;
    bool has_cmd_;
    bool has_crc_;
    struct Footer footer_;
    struct Header header_;
    struct Lenght len_;
    struct CMD cmd_;
    struct CRC crc_;
    std::vector<Type> packet_sections_;
};


#endif // TCPMESSAGEEXTRACTOR_H
