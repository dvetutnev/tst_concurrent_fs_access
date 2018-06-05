/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"
#include "fs/exception.h"

#include <vector>


namespace oda {
namespace fs {


Path currentDirectory();


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
    Default = True
#else
    Default = False
#endif
};

Directory readDirectory(const Path&, CaseSensitive caseSensitive = CaseSensitive::Default);


} // namespace fs
} // namespace oda
