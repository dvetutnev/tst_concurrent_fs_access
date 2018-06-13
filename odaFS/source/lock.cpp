/*
 * ODA filesystem. Locks.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "lock.h"


#include <mutex>
#include <shared_mutex>
#include <unordered_map>


namespace oda {
namespace fs {
namespace internal {


#ifndef ODA_FS_FAKE_LOCK


struct LockItem
{
    mutable std::mutex top;
    mutable std::shared_mutex bottom;
};

static std::unordered_map< Path::string_type, std::shared_ptr<LockItem> > locks;

static std::mutex mutexLockTable;


static std::shared_ptr<LockItem> getLockItem(const Path&);


SharedLock::SharedLock(const Path& path) {

    _lockItem = getLockItem(path);
    _lockItem->top.lock();
    _lockItem->bottom.lock_shared();
    _lockItem->top.unlock();
}

SharedLock::~SharedLock() {

    _lockItem->bottom.unlock_shared();
}


UniqueLock::UniqueLock(const Path& path) {

    _lockItem = getLockItem(path);
    _lockItem->top.lock();
    _lockItem->bottom.lock();
    _lockItem->top.unlock();
}


UniqueLock::~UniqueLock() {

    _lockItem->bottom.unlock();
}


static std::shared_ptr<LockItem> getLockItem(const Path& path) {

    const auto& key = path.native();

    std::lock_guard<std::mutex> lockTable{mutexLockTable};
    std::shared_ptr<LockItem> lockItem = locks[key];
    if (!lockItem) {

        lockItem = std::make_shared<LockItem>();
        locks[key] = lockItem;
    }

    return lockItem;
}

#endif


} // namespace internal
} // namespace fs
} // namespace oda
