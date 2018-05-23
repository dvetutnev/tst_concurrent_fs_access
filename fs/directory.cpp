/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"

#include <boost/filesystem/convenience.hpp>

#include <iostream>


namespace oda {
namespace fs {


Directory readDirectory(const Path& basePath) {

    Directory result;

    try {

        boost::filesystem::directory_iterator it{basePath};
        while (it != boost::filesystem::directory_iterator{}) {

            const auto& path = it->path();
            const auto& fileStatus = it->status();
            const bool isDirectory = fileStatus.type() == boost::filesystem::directory_file;

            result.push_back(DirectoryEntry{path, isDirectory});

            ++it;
        }
    } catch (const boost::filesystem::filesystem_error& e) {

        const boost::system::error_code& boostEc = e.code();
        const std::error_code stdEc = std::make_error_code(static_cast<std::errc>(boostEc.value()));
        throw Exception{stdEc, basePath.string()};
    }

    return result;
}


} // namespace fs
} // namespace oda
