/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"

#include <boost/filesystem/convenience.hpp>


//static bool


namespace oda {
namespace fs {


Directory readDirectory(const Path& p) {

    Directory result;

    boost::filesystem::directory_iterator it{p};
    while (it != boost::filesystem::directory_iterator{}) {

        const auto& path = it->path();
        const auto& fileStatus = it->status();
        const bool isDirectory = fileStatus.type() == boost::filesystem::directory_file;

        result.push_back(DirectoryEntry{path, isDirectory});

        ++it;
    }

    return std::move(result);
}


} // namespace fs
} // namespace oda
