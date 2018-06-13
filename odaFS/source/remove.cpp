/*
 * ODA filesystem. Remove operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/remove.h"
#include "fs/error.h"
#include "lock.h"

#include <boost/filesystem/operations.hpp>


namespace oda {
namespace fs {


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


} // namespace fs
} // namespace oda
