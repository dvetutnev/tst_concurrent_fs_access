#include "data_set.h"
#include "generate.h"

#include <cstdlib>


void DataSet::init() {

    _storage.reserve(_count);
    _keys.reserve(_count);

    for (std::size_t i = 1; i <= _count; i++) {

        std::string data;
        data.resize(_length);
        generate_data(&data[0], _length);

        const std::string key = "data_" + std::to_string(i) + ".dat";
        _storage[key].data = data;

        _keys.push_back(key);
    }
}


void DataSet::update(const std::string& key, const std::string& data) {

    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::unique_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    item.data = data;
}


bool DataSet::compare(const std::string& key, const std::string& data) const {

    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::shared_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    return data == item.data;
}


std::string DataSet::getData(const std::string& key) const {

    auto& item = _storage.at(key);

    item.topMtx.lock();
    std::shared_lock<std::shared_mutex> lock{item.bottomMtx};
    item.topMtx.unlock();

    return std::string{item.data};
}


const std::string& DataSet::getRandomKey() const {

    auto i = (std::rand() * (_count - 1)) / RAND_MAX;
    return _keys[i];
}
