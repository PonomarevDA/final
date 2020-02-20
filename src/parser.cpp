#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

#include "parser.hpp"

static const char response_404[] = "HTTP/1.0 404 NOT FOUND\r\nContent-length: 0\r\nContent-Type: text/html\r\n\r\n";

void create_response_on_get(char data[], size_t& bytes_transferred, const std::string& file_path){
    std::ifstream file(file_path.c_str());
    std::string response;

    if(file.is_open()){
        const int begin = file.tellg();
        file.seekg(0, file.end);
        const int size = file.tellg();
        file.seekg (0, file.beg);

        std::stringstream ss;
        ss << "HTTP/1.0 200 OK\r\n";
        ss << "Content-length: " << size << "\r\n";
        ss << "Content-Type: text/html\r\n";
        ss << "\r\n";
        ss << file.rdbuf();
        file.close();
        response = ss.str();
    }else{
        response = response_404;
    }

    memcpy(data, response.c_str(), response.size());
    bytes_transferred = response.size();
}

// very simple parser get command with or without parameters
void http_parse_and_make_response(char data[], size_t& bytes_transferred){
    std::string request(data, bytes_transferred);

    std::string start_line = request.substr(0, request.find_first_of("\r\n"));

    size_t left_uri_border = start_line.find(' ') + 1;
    size_t right_uri_border = start_line.find(' ', left_uri_border) - 1;
    std::string URI = start_line.substr(left_uri_border, right_uri_border + 1 - left_uri_border);

    std::string file_path;
    size_t question = URI.find("?");
    if (question != std::string::npos) {
        file_path = URI.substr(0, question);
    }else{
        file_path = URI;
    }

    create_response_on_get(data, bytes_transferred, file_path);
}