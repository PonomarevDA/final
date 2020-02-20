#include <iostream>
#include <string>
#include <unistd.h>
#include "parser.hpp"
#include "server.hpp"

static boost::asio::ip::address ip;
static int port;
static std::string directory;

void parse_args(int argc, char** argv){
    // set default values:
    ip = boost::asio::ip::address::from_string("127.0.0.1");
    port = 12345;
    directory = "/";

    int c;
    while((c = getopt(argc, argv, "h:p:d:")) != -1){
        if(optarg){
            switch(c){
                case 'h':
                    ip = boost::asio::ip::address::from_string(optarg);
                    break;
                case 'p':
                    port = atoi(optarg);
                    break;
                case 'd':
                    directory = optarg;
                    break;
            }
        }
    }
}

int main(int argc, char** argv){
    // parse args
    parse_args(argc, argv);
    tcp::endpoint end_point = tcp::endpoint(ip, port);

    // create daemon
    pid_t pid = fork();
    if(pid == -1){
        exit(-1);
    }else if(pid != 0){
        return 0;
    }

    // create server
    boost::asio::io_service io_service;
    Server s(io_service, end_point, directory);
    io_service.run();
    return 0;
}