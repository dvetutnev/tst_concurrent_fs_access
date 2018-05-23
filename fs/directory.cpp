/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"

#include <boost/filesystem/convenience.hpp>

#include <iostream>


namespace oda {
namespace fs {


Directory readDirectory(const Path& path) {

    Directory result;

    try {

        boost::filesystem::directory_iterator it{path};
        while (it != boost::filesystem::directory_iterator{}) {

            const auto& entryPath = it->path();
            const auto& fileStatus = it->status();
            const bool isDirectory = fileStatus.type() == boost::filesystem::directory_file;

            result.push_back(DirectoryEntry{entryPath, isDirectory});

            ++it;
        }
    } catch (const boost::filesystem::filesystem_error& e) {

        const boost::system::error_code& boostEc = e.code();
        const std::error_code stdEc = std::make_error_code(static_cast<std::errc>(boostEc.value()));
        throw Exception{stdEc, path.string()};
    }

    return result;
}


} // namespace fs
} // namespace oda
