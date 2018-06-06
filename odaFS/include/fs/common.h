/*
 * ODA filesystem. Common definitions.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <boost/filesystem/path.hpp>
#include <boost/container_hash/hash.hpp>


namespace oda {
namespace fs {


using Path = boost::filesystem::path;

inline std::size_t hashValue(const Path& path) {

    return boost::filesystem::hash_value(path);
}


} // namespace fs
} // namespace oda
