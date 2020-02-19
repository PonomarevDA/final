#include <iostream>
#include <string.h>
#include <fstream>

#include "parser.hpp"

static const char response_200[] = "HTTP/1.0 200 OK\r\nContent-length: 7\r\nContent-Type: text/html\r\n\r\n";
static const char response_404[] = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";

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
    std::ifstream file(file_path);
    if(file.is_open()){
        response = "HTTP/1.0 200 OK\r\nContent-length: ";

        const auto begin = file.tellg();
        file.seekg(0, file.end);
        //const auto end = file.tellg();
        const auto size = file.tellg();//(end-begin);
        file.seekg (0, file.beg);

        response.append(std::to_string(size));
        response.append("\r\nContent-Type: text/html\r\n");
        std::string buf;
        while(file >> buf){
            response.append(buf);
        }
    }else{
        response = response_200;
    }
}

void set_directory(std::string& dir){
    directory = dir;
}

const std::string& get_directory(){
    return directory;
}