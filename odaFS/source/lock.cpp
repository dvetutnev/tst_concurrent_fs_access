/*
 * ODA filesystem. Locks.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "lock.h"


namespace oda {
namespace fs {
namespace internal {


#ifndef ODA_FS_FAKE_LOCK


SharedLock::SharedLock(const Path& path)
    : _key{path.native()}
{
}


UniqueLock::UniqueLock(const Path& path)
    : _key{path.native()}
{
}


#endif


}
} // namespace fs
} // namespace oda
