#ifndef TCPMESSAGEEXTRACTOR_H
#define TCPMESSAGEEXTRACTOR_H

#include "abstract.h"

class TCPMessageExtractor {
public:
    TCPMessageExtractor(std::shared_ptr<AbstractRawExtractor> extractor, std::shared_ptr<AbstractBuffer> buffer);
    std::shared_ptr<AbstractSerializableMessage> find_message();
private:
    std::shared_ptr<AbstractRawExtractor> extractor_;
    std::shared_ptr<AbstractBuffer> buffer_;
    void find_header();
    void find_cmd();
    void find_len();
    //Extractor
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
};


#endif // TCPMESSAGEEXTRACTOR_H
