#include <iostream>
#include <string>
#include <unistd.h>
#include "server.hpp"

static boost::asio::ip::address ip;
static int port;
static std::string directory;

void parse_args(int argc, char** argv){
    // set default values:
    ip = boost::asio::ip::address::from_string("127.0.0.1");
    port = 12345;

    int c;
    while((c = getopt(argc, argv, "h:p:d:")) != -1){
        switch(c){
            case 'h':
                if(optarg){
                    ip = boost::asio::ip::address::from_string(optarg);
                    std::cout << "-h with ip = " << ip << "." << std::endl;
                }else{
                    std::cout << "-h without ip :(" << std::endl;
                }
                break;
            case 'p':
                if(optarg){
                    port = atoi(optarg);
                    std::cout << "-p with port = " << port << "." << std::endl;
                }else{
                    std::cout << "-p without port :(" << std::endl;
                }
                break;
            case 'd':
                if(optarg){
                    std::cout << "-d with directory = " << optarg << "." << std::endl;
                }else{
                    std::cout << "-d without directory :(" << std::endl;
                }
                break;
        }
    }
}

int main(int argc, char** argv){
    parse_args(argc, argv);
    auto end_point = tcp::endpoint(ip, port);

    boost::asio::io_service io_service;
    using namespace std;
    Server s(io_service, end_point);
    io_service.run();
    return 0;
}