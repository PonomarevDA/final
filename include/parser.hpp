#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>

void http_parse_and_make_response(char data[], size_t& bytes_transferred);
void create_response_on_get(std::string& response, const std::string& file_path);
void set_directory(std::string& dir);
const std::string& get_directory();

#endif // PARSER_HPP_