/*
 * ODA filesystem. Files IO operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/file.h"

#include <boost/filesystem/fstream.hpp>
#include <cassert>


constexpr std::size_t bufferLength = 4096;


namespace oda {
namespace fs {


std::string readFile(const Path& path) {


    boost::filesystem::ifstream file{path, std::ios_base::binary};

    std::string result;
    std::string buffer;
    buffer.resize(bufferLength);

    while (file) {

        file.read(&buffer[0], bufferLength);
        const auto count = file.gcount();
        assert(count >= 0);
        result.append(buffer.data(), static_cast<std::size_t>(count));
    }
    if (!file.eof()) {

        const std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
        throw Exception{ec, path.string()};
    }

    return result;
}


void writeFile(const Path& path, const std::string& data, std::ios_base::openmode mode) {

    boost::filesystem::ofstream file{path, std::ios_base::binary | mode};
    if (!file) {

        const std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
        throw Exception{ec, path.string()};
    }
    file << data;
}


} // namespace fs
} // namespace oda
