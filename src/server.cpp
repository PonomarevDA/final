#include "server.hpp"


Session::Session(boost::asio::io_service& io_service): socket_(io_service) {}

void Session::start(){
    socket_.async_read_some(
        boost::asio::buffer(data_, MAX_LENGTH),
        boost::bind(
            &Session::handle_read,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred){
    if(!error){
        std::cout << "Session: heard: \"" << data_ << "\"" << std::endl;
        std::cout << "Session: send: \"" << data_ << "\"" << std::endl;
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(data_, bytes_transferred),
            boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
    }else{
        delete this;
    }
}

void Session::handle_write(const boost::system::error_code& error){
    if(!error){
        socket_.async_read_some(
            boost::asio::buffer(data_, MAX_LENGTH),
            boost::bind(
                &Session::handle_read,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }else{
        delete this;
    }
}



Server::Server(boost::asio::io_service& io_service, tcp::endpoint end_point)
    : io_service_(io_service), acceptor_(io_service, end_point) {
    start_accept();
}

void Server::start_accept(){
    std::cout << "Server: create session and accept." << std::endl;
    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(
        new_session->get_socket(),
        boost::bind(&Server::handle_accept, this, new_session, 
                    boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error){
    if(!error){
        new_session->start();
    }else{
        delete new_session;
    }
    start_accept();
}