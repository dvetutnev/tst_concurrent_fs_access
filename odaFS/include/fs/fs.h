/*
 * ODA filesystem.
 * Dmitriy Vetutnev, ODANT 2018
*/

#pragma once

#ifdef _WIN32
    #ifdef ODA_FS_BUILD
        #define ODA_FS_EXPORT __declspec(dllexport)
    #else
        #define ODA_FS_EXPORT __declspec(dllimport)
    #endif
#else
    #define ODA_FS_EXPORT
#endif


#include <common/exception.h>

#include <boost/filesystem/path.hpp>

#include <vector>
#include <string>


namespace oda {
namespace fs {


class Exception : public ::oda::error::Exception
{
    using ::oda::error::Exception::Exception;
};

using Path = boost::filesystem::path;

// Directory operations
struct DirectoryEntry
{
    Path path;
    bool isDirectory;
};

inline bool operator== (const DirectoryEntry& lhs, const DirectoryEntry& rhs) {

    return lhs.path == rhs.path && lhs.isDirectory == rhs.isDirectory;
}

inline bool operator< (const DirectoryEntry& lhs, const DirectoryEntry& rhs) {

    return lhs.path < rhs.path;
}

using Directory = std::vector<DirectoryEntry>;

enum class CaseSensitive
{
    True,
    False,
#ifdef _WIN32
    Default = False
#else
    Default = True
#endif
};

ODA_FS_EXPORT Directory readDirectory(const Path&, CaseSensitive caseSensitive = CaseSensitive::Default);
ODA_FS_EXPORT Path currentDirectory();
ODA_FS_EXPORT void createDirectory(const Path&);

// Commnon operations
ODA_FS_EXPORT void remove(const Path&);
ODA_FS_EXPORT void removeAll(const Path&);
ODA_FS_EXPORT void rename(const Path&, const Path&);

// File operations
ODA_FS_EXPORT std::string readFile(const Path&);
ODA_FS_EXPORT void writeFile(const Path&, const std::string&, std::ios_base::openmode);


#ifdef ODA_FS_TESTS
namespace internal {

ODA_FS_EXPORT void* addressOfLocks();
ODA_FS_EXPORT std::size_t sizeOfLocks();
ODA_FS_EXPORT void insertDummyLock();

} // namespace internal
#endif


} // namespace fs
} // namespace oda
