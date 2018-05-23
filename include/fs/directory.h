/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"

#include <vector>


namespace oda {
namespace fs {


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


Directory readDirectory(const Path&);


} // namespace fs
} // namespace oda
