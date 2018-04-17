#include "fs_lock.h"


#include <map>
#include <mutex>
#include <shared_mutex>
#include <cassert>


namespace v_0_0 {

static std::map<std::string, std::shared_mutex> table;
static std::mutex mtx_table;

shared_lock::shared_lock(const std::string& filepath)
    : _ptr_mtx_file{nullptr}
{

    std::lock_guard<std::mutex> lock_table{mtx_table};
    _ptr_mtx_file = &(table[filepath]);
    _ptr_mtx_file->lock_shared();
}


shared_lock::~shared_lock() {

    assert(_ptr_mtx_file != nullptr);
    _ptr_mtx_file->unlock_shared();
}

}   // namespace v_0_0
