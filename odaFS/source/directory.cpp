/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"
#include "glob.h"

#include <iostream>

namespace oda {
namespace fs {


Path currentDirectory() {

    return boost::filesystem::current_path();
}


Directory readDirectory(const Path& path, CaseSensitive caseSensitive) {

    Directory result;

    try {

        internal::Glob glob{path, caseSensitive};
        boost::filesystem::directory_iterator it{glob.dirPath()};
        const boost::filesystem::directory_iterator end;

        for (; it != end; ++it) {

            const auto& entryPath = it->path();

            if (glob.match(entryPath)) {

                const auto& fileStatus = it->status();
                const bool isDirectory = fileStatus.type() == boost::filesystem::directory_file;

                result.push_back(DirectoryEntry{entryPath, isDirectory});
            }
        }
    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path.string());
    }

    return result;
}


void createDirectory(const Path& path) {

    try {

        boost::filesystem::create_directory(path);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path.string());
    }
}


} // namespace fs
} // namespace oda
