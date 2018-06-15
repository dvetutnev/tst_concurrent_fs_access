/*
 * ODA filesystem. Locks.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "lock.h"
#include "fs/fs.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <unordered_map>
#include <memory>


namespace oda {
namespace fs {
namespace internal {


#ifndef ODA_FS_FAKE_LOCK


struct LockItem
{
    mutable boost::mutex top;
    mutable boost::shared_mutex bottom;
};

static std::unordered_map< Path::string_type, std::shared_ptr<LockItem> > locks;

static boost::mutex mutexLockTable;


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

    boost::lock_guard<boost::mutex> lockTable{mutexLockTable};
    std::shared_ptr<LockItem> lockItem = locks[key];
    if (!lockItem) {

        lockItem = std::make_shared<LockItem>();
        locks[key] = lockItem;
    }

    return lockItem;
}


void* addressOfLocks() {

    return std::addressof(locks);
}

std::size_t sizeOfLocks() {

    return locks.size();
}

void insertDummyLock() {

    const Path path = "aaa.bbb";
    locks[path.native()] = std::shared_ptr<LockItem>{};
}

#endif


} // namespace internal
} // namespace fs
} // namespace oda
