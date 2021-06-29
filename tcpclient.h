#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

#include <boost/mpl/size.hpp>
#include "abstract.h"
#include "buffer_base.h"
#include "tcpmessageextractor.h"
#include "buffer_template.h"

#define MAX_LENGTH (256 * 4)

enum class TCPError{
    Server
};


typedef std::shared_ptr<boost::asio::ip::tcp::socket> TCPSocketShared;
class TCPClient {
public:
    TCPClient(TCPSocketShared socket);
    TCPClient(std::string ip, short port);
    void set_buffer(std::shared_ptr<AbstractBuffer> buffer);
    void set_extractor(std::shared_ptr<AbstractRawExtractor> extractor);
    bool start();
    void set_buffer_size(uint32_t size_bytes);
    void disconnect();
    void async_send(const char* data, const uint32_t size, std::function<void (int)> func);
    size_t send(const char* data, const uint32_t size);


    boost::signals2::connection notify_me_when_disconnected(std::function<void (int) >func);
    std::string get_ip() const;
    bool is_connected() const;
    short get_port() const;
    int get_id() const;

    std::shared_ptr<AbstractSerializableMessage> get_next_packet();
    BufferError get_next_bytes(uint8_t *data, const uint32_t len, const uint32_t timeout_ms = 0);
    uint8_t get_next_byte();
    void start_print_send_receive_rate();
    ~TCPClient();
private:
    void initialize();
    void handle_read_data(const boost::system::error_code error, const size_t bytes_transferred);
    void handle_write(const boost::system::error_code &error, const size_t byte_transfer);
    void io_contex_thread();
    void extract_message();

    std::string ip_;
    short port_;
    std::shared_ptr<AbstractBuffer> buffer_;
    uint32_t buffer_size_;
    long int receive_size_;
    bool is_connected_;
    bool is_running_;
    int id_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    boost::shared_ptr<boost::asio::io_context::work> work_;

    std::array<uint8_t, MAX_LENGTH> data_;
    boost::signals2::signal <void (int)> disconnect_connections_;
    long int send_size_;
    boost::thread_group thread_group_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::io_context io_context_;
    static std::atomic<int> ID_Counter_;


    // Extractor parameters
//    std::shared_ptr<Buffer<std::shared_ptr<AbstractMessage>>> messages_buffer_;
    std::shared_ptr<AbstractRawExtractor> extractor_;

};

typedef std::shared_ptr<TCPClient> TCPClientShared;

#endif // TCPCLIENT_H
