/*
 * ODA filesystem. Locks.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/fs.h"

#include <memory>


namespace oda {
namespace fs {
namespace internal {


#ifdef ODA_FS_FAKE_LOCK

struct SharedLock
{
    SharedLock(const Path&) {}
};


struct UniqueLock
{
    UniqueLock(const Path&) {}
};

#endif

#ifndef ODA_FS_FAKE_LOCK


struct LockItem;


class SharedLock
{
public:

    SharedLock(const Path&);
    ~SharedLock();

private:

    std::shared_ptr<LockItem> _lockItem;
};


class UniqueLock
{
public:

    UniqueLock(const Path&);
    ~UniqueLock();

private:

    std::shared_ptr<LockItem> _lockItem;
};

#endif


} // namespace internal
} // namespace fs
} // namespace oda
