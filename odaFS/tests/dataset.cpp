#include "dataset.h"

#include <cstdlib>


void DataSet::init() {

    _storage.reserve(_count);
    _paths.reserve(_count);

    const oda::fs::Path dir = "dataset";

    for (std::size_t i = 1; i <= _count; i++) {

        std::string data;
        data.resize(_length);
        generate_data(&data[0], _length);

        const oda::fs::Path filepath = "data_" + std::to_string(i) + ".dat";
        const auto& key = filepath.native();
        _storage[key].data.swap(data);

        _paths.push_back(filepath);
    }
}


void DataSet::update(const oda::fs::Path& path, const std::string& data) {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::unique_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    item.data = data;
}


bool DataSet::compare(const oda::fs::Path& path, const std::string& data) const {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::shared_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    return data == item.data;
}


std::string DataSet::getData(const oda::fs::Path& path) const {

    const auto& key = path.native();
    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::shared_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    return std::string{item.data};
}


const oda::fs::Path& DataSet::getRandomPath() const {

    auto i = (std::rand() * (_count - 1)) / RAND_MAX;
    return _paths[i];
}


const std::vector<oda::fs::Path>& DataSet::getAllPaths() const {

    return _paths;
}

void generate_data(char* buffer, std::size_t length) {

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
