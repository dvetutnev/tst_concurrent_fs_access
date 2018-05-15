#include "generate.h"

#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <memory>


void generate_data(char* buffer, std::size_t length) {

    auto rd = []() {
        return static_cast<char>(std::rand());
    };
    std::generate_n(buffer, length, rd);
}


void generate_file(const std::string& name, std::size_t length) {

    auto mode = std::ios::binary | std::ios::out | std::ios::trunc;
    std::fstream file{name, mode};

    auto buffer = std::make_unique<char[]>(length);
    generate_data(buffer.get(), length);

    file.write(buffer.get(), length);
}
