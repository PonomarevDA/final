#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

#include "parser.hpp"

static const char response_404[] = "HTTP/1.0 404 NOT FOUND\r\nContent-length: 0\r\nContent-Type: text/html\r\n\r\n";

static std::string directory;

// very simple parser
void http_parse_and_make_response(char data[], size_t& bytes_transferred){
    // define file name
    char* const dir_left = &data[4];
    char* dir_right = &data[5];
    const char* const end = data + bytes_transferred;
    while((*dir_right != ' ') && (dir_right < end)){
        dir_right++;
    }

    // check if error request
    if(dir_right == end || *dir_right != ' '){
        //std::cout << "can't find dir :(" << std::endl;
        memcpy(data, response_404, sizeof(response_404));
        bytes_transferred = sizeof(response_404);
        return;
    }

    // define full path of file
    std::string path = get_directory();
    path.append(dir_left, dir_right - dir_left);
    //std::cout << "path: " << path << std::endl;

    // create response
    std::string response;
    create_response_on_get(response, path);

    memcpy(data, response.c_str(), response.size());
    bytes_transferred = response.size();
}

void create_response_on_get(std::string& response, const std::string& file_path){
    std::ifstream file(file_path.c_str());

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

        std::string buf;
        while(file >> buf){
            ss << buf;
        }
        response = ss.str();
    }else{
        response = response_404;
    }
}

void set_directory(std::string& dir){
    directory = dir;
}

const std::string& get_directory(){
    return directory;
}