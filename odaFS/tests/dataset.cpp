#include "dataset.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include <cassert>
#include <cstdlib>


void DataSet::init() {

    assert(_count >= 1);

    _storage.reserve(_count);
    _paths.reserve(_count);

    const oda::fs::Path dir = "dataset";

    for (std::size_t i = 1; i <= _count; i++) {

        std::string data;
        data.resize(_length);
        generateData(&data[0], _length);

        const oda::fs::Path filepath = "data_" + std::to_string(i) + ".dat";
        const auto& key = filepath.native();
        _storage[key].data.swap(data);

        _paths.push_back(filepath);
    }
}


std::unique_ptr<DataSet::UniqueLock> DataSet::update(const oda::fs::Path& path, const std::string& data) {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    auto lock = std::make_unique<UniqueLock>(item);

    item.data = data;

    return lock;
}


bool DataSet::compare(const oda::fs::Path& path, const std::string& data) const {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    auto lock = std::make_unique<SharedLock>(item);

    return data == item.data;
}


std::string DataSet::getData(const oda::fs::Path& path) const {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    auto lock = std::make_unique<SharedLock>(item);

    return std::string{item.data};
}


const oda::fs::Path& DataSet::getRandomPath() const {

    const std::size_t i = (static_cast<std::size_t>(std::rand()) * (_count - 1)) / RAND_MAX;
    return _paths[i];
}


const std::vector<oda::fs::Path>& DataSet::getAllPaths() const {

    return _paths;
}


void DataSetFiles::init() {

    const auto& paths = _dataSet.getAllPaths();
    for (const oda::fs::Path& path : paths) {

        if (boost::filesystem::exists(path)) {

            boost::filesystem::remove_all(path);
        }
        boost::filesystem::ofstream file{path, std::ios_base::binary};
        const std::string& content = _dataSet.getData(path);
        const char* content_data = content.data();
        const auto content_length = content.length();
        file.write(content_data, static_cast<std::streamsize>(content_length));
    }
}


DataSetFiles::~DataSetFiles() {

    const auto& paths = _dataSet.getAllPaths();
    for (const oda::fs::Path& path : paths) {

        if (boost::filesystem::exists(path)) {

            boost::filesystem::remove_all(path);
        }
    }
}


DataSet::SharedLock::SharedLock(const Item& item)
    : _item{item}
{
    _item.topMtx.lock();
    _item.bottomMtx.lock_shared();
    _item.topMtx.unlock();
}

DataSet::SharedLock::~SharedLock() {

    _item.bottomMtx.unlock_shared();
}

DataSet::UniqueLock::UniqueLock(const Item& item)
    : _item{item}
{
    _item.topMtx.lock();
    _item.bottomMtx.lock();
    _item.topMtx.unlock();
}

DataSet::UniqueLock::~UniqueLock() {

    _item.bottomMtx.unlock();
}


void generateData(char* buffer, std::size_t length) {

    int* const start = reinterpret_cast<int*>(buffer);
    int* const end = start + length / sizeof(int);

    for (int* i = start; i < end; i++) {

        *i = static_cast<int>(std::rand());
    }


    char* const startTail = reinterpret_cast<char*>(end);
    char* const endTail = startTail + length % sizeof(int);

    for (char* i = startTail; i < endTail; i++) {

        *i = static_cast<char>(std::rand());
    }
}
