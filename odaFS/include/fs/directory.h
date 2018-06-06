/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"
#include "fs/error.h"

#include <vector>


namespace oda {
namespace fs {


Path currentDirectory();

// readDirectory
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

void createDirectory(const Path&);


} // namespace fs
} // namespace oda
