/*
 * ODA filesystem. Locks.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"


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

class SharedLock
{
public:

    SharedLock(const Path&);

private:

    const Path::string_type& _key;
};


class UniqueLock
{
public:

    UniqueLock(const Path&);

private:

    const Path::string_type& _key;
};

#endif


}
} // namespace fs
} // namespace oda
