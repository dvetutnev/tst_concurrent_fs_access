/*
 * ODA filesystem.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/fs.h"
#include "glob.h"
#include "lock.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>


namespace oda {
namespace fs {

namespace internal {

constexpr std::size_t bufferLength = 4096;

void reThrow(const boost::filesystem::filesystem_error&, const Path&);

} // namespace internal


// Directory operations
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

        internal::reThrow(ex, path);
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


// Common operations
void remove(const Path& path) {

    try {

        internal::UniqueLock lock{path};
        boost::filesystem::remove(path);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path);
    }
}


void removeAll(const Path& path) {

    try {

        internal::UniqueLock lock{path};
        boost::filesystem::remove_all(path);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path);
    }
}


void rename(const Path& oldPath, const Path& newPath) {

    try {

        internal::UniqueLock oldLock{oldPath};
        internal::UniqueLock newLock{newPath};
        boost::filesystem::rename(oldPath, newPath);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, oldPath);
    }
}


// File operations
std::string readFile(const Path& path) {

    internal::SharedLock lock{path};

    boost::filesystem::ifstream file{path, std::ios_base::binary};

    std::string result;
    std::string buffer;
    buffer.resize(internal::bufferLength);

    while (file) {

        file.read(&buffer[0], internal::bufferLength);
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

    internal::UniqueLock lock{path};

    boost::filesystem::ofstream file{path, std::ios_base::binary | mode};
    if (!file) {

        const std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
        throw Exception{ec, path.string()};
    }
    file << data;
}


namespace internal {

void reThrow(const boost::filesystem::filesystem_error& ex, const Path& path) {

    const boost::system::error_code& boostEc = ex.code();
#ifdef _WIN32
    const std::error_code stdEc{boostEc.value(), std::system_category()};
#else
    const std::error_code stdEc{boostEc.value(), std::generic_category()};
#endif
    throw Exception{stdEc, path.string()};
}

} // namespace internal


} // namespace fs
} // namespace oda
