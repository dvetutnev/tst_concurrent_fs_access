/*
 * ODA filesystem.
 * Dmitriy Vetutnev, ODANT 2018
*/

#pragma once


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

Directory readDirectory(const Path&, CaseSensitive caseSensitive = CaseSensitive::Default);
Path currentDirectory();
void createDirectory(const Path&);

// Commnon operations
void remove(const Path&);
void removeAll(const Path&);
void rename(const Path&, const Path&);

// File operations
std::string readFile(const Path&);
void writeFile(const Path&, const std::string&, std::ios_base::openmode);


namespace internal {

void* addressOfLocks();

} // namespace internal


} // namespace fs
} // namespace oda
