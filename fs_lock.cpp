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


namespace v_0_1 {

static std::map< std::string, std::shared_ptr<std::shared_mutex> > table;
static std::mutex mtx_table;

shared_lock::shared_lock(const std::string& filepath)
    : _ptr_mtx_file{nullptr}
{

    std::lock_guard<std::mutex> lock_table{mtx_table};
    _ptr_mtx_file = table[filepath];
    if (!_ptr_mtx_file) {
        _ptr_mtx_file = std::make_shared<std::shared_mutex>();
        table[filepath] = _ptr_mtx_file;
    }
    _ptr_mtx_file->lock_shared();
}


shared_lock::~shared_lock() {

    assert(_ptr_mtx_file != nullptr);
    _ptr_mtx_file->unlock_shared();
}


unique_lock::unique_lock(const std::string& filepath)
    : _ptr_mtx_file{nullptr}
{

    std::lock_guard<std::mutex> lock_table{mtx_table};
    _ptr_mtx_file = table[filepath];
    if (!_ptr_mtx_file) {
        _ptr_mtx_file = std::make_shared<std::shared_mutex>();
        table[filepath] = _ptr_mtx_file;
    }
    _ptr_mtx_file->lock();
}


unique_lock::~unique_lock() {

    assert(_ptr_mtx_file != nullptr);
    _ptr_mtx_file->unlock();
}

}   // namespace v_0_1


namespace v_0_2 {

static std::map< std::string, std::shared_ptr<std::shared_mutex> > table;
static std::mutex mtx_table;

shared_lock::shared_lock(const std::string& filepath)
    : _ptr_mtx_file{nullptr}
{
    {
        std::lock_guard<std::mutex> lock_table{mtx_table};
        _ptr_mtx_file = table[filepath];
        if (!_ptr_mtx_file) {
            _ptr_mtx_file = std::make_shared<std::shared_mutex>();
            table[filepath] = _ptr_mtx_file;
        }
    }
    _ptr_mtx_file->lock_shared();
}


shared_lock::~shared_lock() {

    assert(_ptr_mtx_file != nullptr);
    _ptr_mtx_file->unlock_shared();
}


unique_lock::unique_lock(const std::string& filepath)
    : _ptr_mtx_file{nullptr}
{
    {
        std::lock_guard<std::mutex> lock_table{mtx_table};
        _ptr_mtx_file = table[filepath];
        if (!_ptr_mtx_file) {
            _ptr_mtx_file = std::make_shared<std::shared_mutex>();
            table[filepath] = _ptr_mtx_file;
        }
    }
    _ptr_mtx_file->lock();
}


unique_lock::~unique_lock() {

    assert(_ptr_mtx_file != nullptr);
    _ptr_mtx_file->unlock();
}

}   // namespace v_0_2


namespace v_1_0 {

struct Locks
{
    std::mutex top;
    std::shared_mutex bottom;
};


static std::map< std::string, std::shared_ptr<Locks> > table;
static std::mutex mtx_table;


shared_lock::shared_lock(const std::string& filepath) {

    {
        std::lock_guard<std::mutex> lock_table{mtx_table};
        _locks = table[filepath];
        if (!_locks) {
            _locks = std::make_shared<Locks>();
            table[filepath] = _locks;
        }
    }

    std::lock_guard<std::mutex> lock_top{_locks->top};
    _locks->bottom.lock_shared();
}


shared_lock::~shared_lock() {

   _locks->bottom.unlock_shared();
}


unique_lock::unique_lock(const std::string& filepath) {

    {
        std::lock_guard<std::mutex> lock_table{mtx_table};
        _locks = table[filepath];
        if (!_locks) {
            _locks = std::make_shared<Locks>();
            table[filepath] = _locks;
        }
    }

    std::lock_guard<std::mutex> lock_top{_locks->top};
    _locks->bottom.lock();
}


unique_lock::~unique_lock() {

    _locks->bottom.unlock();
}

}   // namespace v_1_0
