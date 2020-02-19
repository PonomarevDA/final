#include <fstream>
#include <iostream>

static std::ofstream log_file;

void log_data(char data[], size_t size){
    log_file.open("log.txt", std::ios_base::app);
    if(log_file.is_open()){
        log_file << std::string(data, size);
        log_file.close();
    }
}