#include "generate.h"

#include <fstream>
//#include <algorithm>
#include <cstdlib>
#include <memory>


void generate_data(char* buffer, std::size_t length) {

    //auto rd = []() {
        //return static_cast<char>(std::rand());
    //};
    //std::generate_n(buffer, length, rd);


    int* const start = reinterpret_cast<int*>(buffer);
    int* const end = start + length / sizeof(int);

    for (int* i = start; i < end; i++) {

        *i = static_cast<int>(std::rand());
    }


    char* const startTail = reinterpret_cast<char*>(end);
    char* const endTail = startTail + length % sizeof(int);

    for (char* i = startTail; i < endTail; i++) {

        *i = static_cast<char>(std::rand());
    }
}


void generate_file(const std::string& name, std::size_t length) {

    auto mode = std::ios::binary | std::ios::out | std::ios::trunc;
    std::fstream file{name, mode};

    auto buffer = std::make_unique<char[]>(length);
    generate_data(buffer.get(), length);

    file.write(buffer.get(), length);
}
