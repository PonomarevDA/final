#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session{
public:
    Session(boost::asio::io_service& io_service);
    tcp::socket& get_socket() { return socket_; }
    void start();
    void handle_read(const boost::system::error_code&, size_t);
    void handle_write(const boost::system::error_code& error);
private:
    tcp::socket socket_;
    enum{ MAX_LENGTH = 1024 };
    char data_[MAX_LENGTH];
};

class Server{
public:
    Server(boost::asio::io_service& io_service, tcp::endpoint end_point, std::string directory);
private:
    void start_accept();
    void handle_accept(Session* new_session, const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

#endif // SERVER_HPP_