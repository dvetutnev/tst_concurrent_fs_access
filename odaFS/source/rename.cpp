/*
 * ODA filesystem. Rename operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/remove.h"
#include "fs/error.h"
#include "lock.h"

#include <boost/filesystem/operations.hpp>


namespace oda {
namespace fs {

void rename(const Path& oldPath, const Path& newPath) {

    try {

        internal::UniqueLock oldLock{oldPath};
        internal::UniqueLock newLock{newPath};
        boost::filesystem::rename(oldPath, newPath);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, oldPath);
    }
}


} // namespace fs
} // namespace oda
