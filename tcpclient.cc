#include "tcpclient.h"
#include <iostream>
#include <boost/asio/detail/posix_thread.hpp>
#include "fast_buffer.h"
#include "circular_buffer.h"

std::atomic<int> TCPClient::ID_Counter_(0);

TCPClient::TCPClient(TCPSocketShared socket)
    : ip_(socket->remote_endpoint().address().to_string()), port_(socket->remote_endpoint().port()), socket_(socket)
{
    initialize();
    is_connected_ = true;
//    messages_buffer_.resize(200);
}

TCPClient::TCPClient(std::string ip, short port)
    : ip_(ip), port_(port)
{
    work_ = boost::make_shared<boost::asio::io_context::work>(io_context_);
    socket_ = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    initialize();
    is_connected_ = false;
}

void TCPClient::set_buffer(std::shared_ptr<AbstractBuffer> buffer)
{
    buffer_ = buffer;
}

void TCPClient::set_extractor(std::shared_ptr<AbstractRawExtractor> extractor)
{
    extractor_ = extractor;
}

void TCPClient::initialize()
{
    is_running_ = false;
    buffer_size_ = 2 * 1024 * 1024;
    receive_size_ = 0;
    send_size_= 0;
    id_ = ID_Counter_;
    ID_Counter_++;
}

bool TCPClient::start()
{
    if (!is_running_) {
        if (!is_connected_) {
            boost::system::error_code ec;
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip_, std::to_string(port_));
            endpoint_ = *resolver.resolve(query);
            auto error = socket_->connect(endpoint_, ec);
            if (error)
                return false;
            is_connected_ = true;
        }

        if (extractor_ != nullptr && buffer_ == nullptr)
            buffer_ = std::make_shared<FastBuffer>(buffer_size_);
        else if (buffer_ == nullptr)
            buffer_ = std::make_shared<CircularBuffer>(buffer_size_);

        thread_group_.create_thread(boost::bind(&TCPClient::extract_message, this));
        thread_group_.create_thread(boost::bind(&TCPClient::io_contex_thread, this));
        socket_->async_receive(boost::asio::buffer(data_.data(), data_.size()),
                               boost::bind(&TCPClient::handle_read_data, this, boost::asio::placeholders::error ,boost::asio::placeholders::bytes_transferred));
        is_connected_ = true;
        is_running_ = true;
    }
    return true;
}

void TCPClient::set_buffer_size(uint32_t size_bytes)
{
    //    buffer_->set_size(size_bytes);
    buffer_size_ = size_bytes;
}


size_t TCPClient::send(const char *data, const uint32_t size)
{
    if (is_connected_) {
        send_size_ += size;
        return socket_->send(boost::asio::buffer(data, size));
    }
    return -1;
}

void TCPClient::async_send(const char *data, const uint32_t size, std::function<void (int)> func)
{
    if (is_connected_)
        socket_->async_send(boost::asio::buffer(data, size), boost::bind(func, boost::asio::placeholders::bytes_transferred));
}

void TCPClient::handle_read_data(const boost::system::error_code error, const size_t bytes_transferred)
{
    if (!error) {
        if (extractor_ != nullptr) {
            //            fast_buffer_->receive_data((const char*)data_.data(), bytes_transferred);
        } else{
            //            buffer_->write(data_.data(), bytes_transferred);
        }
        socket_->async_receive(boost::asio::buffer(data_.data(), data_.size()),
                               boost::bind(&TCPClient::handle_read_data, this, boost::asio::placeholders::error ,boost::asio::placeholders::bytes_transferred));
    } else {
        if (is_connected_) {
            is_connected_ = false;
            disconnect_connections_(id_);
        }
    }
}

BufferError TCPClient::get_next_bytes(uint8_t *data, const uint32_t len, const uint32_t timeout_ms)
{
    if (extractor_ != nullptr)
        return BufferError::BUF_NODATA;
    auto ret = buffer_->read(data, len, timeout_ms);
    return ret;
}

uint8_t TCPClient::get_next_byte()
{
    uint8_t data;
    uint8_t *data_ptr = &data;
    buffer_->read(data_ptr, 1);
    return data;
}

void TCPClient::handle_write(const boost::system::error_code &error, const size_t byte_transfer)
{
    if (error) {
        //        send_size_ += byte_transfer;
        std::cout << "there is an error for async writing" << std::endl;
    }
}

void TCPClient::extract_message()
{
    while(is_connected_ /* && buffer empty*/) {
        auto msg = extractor_->find_message();
//        messages_buffer_.
    }
}

boost::signals2::connection TCPClient::notify_me_when_disconnected(std::function<void (int)> func)
{
    return disconnect_connections_.connect(func);
}

bool TCPClient::is_connected() const
{
    return socket_->is_open();
}

void TCPClient::disconnect()
{
    socket_->close();
    is_connected_ = false;
    is_running_ = false;
}
void TCPClient::start_print_send_receive_rate()
{
    std::cout << "Data sending " + std::to_string(send_size_    / (1024*1024) ) + " MB/s  " + std::to_string(send_size_) + " Bytes" << std::endl;
    std::cout << "Data receive " + std::to_string(receive_size_ / (1024*1024) ) + " MB/s  " + std::to_string(receive_size_) + " Bytes" << std::endl;
    send_size_ = 0;
    receive_size_ = 0;

}

void TCPClient::io_contex_thread()
{
    io_context_.run();
}


std::string TCPClient::get_ip() const
{
    return ip_;
}

short TCPClient::get_port() const
{
    return port_;
}

int TCPClient::get_id() const
{
    return id_;
}

std::shared_ptr<AbstractMessage> TCPClient::get_next_packet()
{

}

TCPClient::~TCPClient()
{
    if (is_connected_)
        disconnect();
    is_running_ = false;
    if (work_ != nullptr)
        work_.reset();
    thread_group_.join_all();
    std::cout << "client dis" << std::endl;

}

