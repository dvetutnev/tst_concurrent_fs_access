#pragma once

#include <string>

void generate_data(char* buffer, std::size_t length);
void generate_file(const std::string& filename, std::size_t length = 8 * 1024);
