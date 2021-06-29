#pragma once

#include <memory>
#include <boost/signals2.hpp>

#include "buffer_base.h"


enum Type{
    Header  = 0b00000001,
    Lenght  = 0b00000010,
    CMD     = 0b00000100,
    Footer  = 0b00001000,
    CRC     = 0b00010000,
    Data    = 0b00100000,
    Other   = 0b01000000
};

class AbstractPacketSection {
public:
    virtual Type get_type() const = 0;
    virtual int get_start_index() const = 0;
    virtual int get_section_len() const = 0;
};

class HeaderSection : AbstractPacketSection{
public:
    HeaderSection(int len) { len_ = len; }
    Type get_type() const { return Type::Header;}
    int get_start_index() const  { return 0; }
    int get_section_len() const { return len_;}
private:
    int len_;
};

class CMDSection : AbstractPacketSection{
public:
    CMDSection(int len, int start_index) { len_ = len; start_index_ = start_index;}
    Type get_type() const { return Type::CMD;}
    int get_start_index() const  { return start_index_; }
    int get_section_len() const { return len_;}
private:
    int len_;
    int start_index_;
};




struct Lenght {
    int len;
    bool is_msb;
    int include;
};
class AbstractMessageFactory;

struct CMD {
    int len;
    std::shared_ptr<AbstractMessageFactory> factory;
};

struct Header {
    int len;
    std::string content;
};

struct Other {
    int len;
    std::string content;
};

struct Footer {
    int len;
    std::string content;
};

class AbstractCRC{
public:
    virtual bool is_valid(const char* data, size_t data_size, const char* crc_data, size_t crc_size) const = 0;
};

struct CRC{
    int start_index;
    int len;
    int start_data_index;
    int end_data_index;
    std::shared_ptr<AbstractCRC> crc_checker;
};



//!
//! \brief The AbstractSerializableMessage class is an interface which implement the Message and used for any type of message that can serial and deseriale itself.
//!
//!
class AbstractSerializableMessage
{
public :
    //!
    //! \brief serialize method can fill the input buffer by serializing values . the values in serializing and deserializing should be same.
    //! \param buffer for containg the serializing data
    //! \param size is used for available buffer size
    //!
    virtual void serialize(char * buffer, size_t size) = 0;
    //!
    //! \brief deserialize method is used for setting properties and members from buffer as same method as serialize
    //! \param buffer is used for reading values
    //! \param size the values
    //!
    virtual void deserialize(const char * buffer, size_t size) = 0 ;
    //!
    //! \brief getserialSize method return the size of serializing and deserializing method
    //! \return the size_t value which show the serializ size
    //!
    virtual size_t get_serialize_size() = 0;
    virtual int get_type() const = 0;
    virtual bool has_len() const = 0;
    virtual struct Lenght get_len() const = 0;
//    virtual struct Data
    virtual ~AbstractSerializableMessage(){} //! virtual interface destructor
};

class AbstractMessage{

};


class AbstractMessageFactory
{
public:
    virtual std::shared_ptr<AbstractMessage> build_message(const char * cmd) = 0;
};



class AbstractMessageExtractor{
public :
    virtual void receive_data(const uint8_t *buffer, const size_t size) = 0;
    virtual boost::signals2::connection notify_me_when_msg_extract(std::function<void (std::shared_ptr<AbstractMessage> message)>) = 0;
    virtual std::shared_ptr<AbstractMessageFactory> get_factory() const = 0;
};

class AbstractMessagePackager {
public:
    virtual boost::signals2::connection notify_me_when_msg_package(std::function< void (char * buffer, size_t size)>) = 0;
    virtual void package_message(std::shared_ptr<AbstractMessage> message) = 0;
};



class AbstractRawExtractor {
public:
    virtual struct Lenght get_len() const = 0;
    virtual struct Header get_header() const = 0;
    virtual struct CRC get_crc() const = 0;
    virtual struct CMD get_cmd() const = 0;
    virtual struct Footer get_footer() const = 0;
//    virtual struct
    virtual bool has_header() const = 0;
    virtual bool has_len() const = 0;
    virtual bool has_crc() const = 0;
    virtual bool has_cmd() const = 0;
    virtual bool has_foofer() const = 0;
    virtual std::shared_ptr<AbstractMessage> find_message() = 0;
};

