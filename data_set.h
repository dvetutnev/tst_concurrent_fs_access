#pragma once


#include <string>
#include <shared_mutex>
#include <unordered_map>
#include <vector>


class DataSet
{
public:

    struct Item
    {
        std::string data;
        mutable std::shared_mutex mtx;
    };

    DataSet(std::size_t count, std::size_t length = 8 * 1024)
        : _count{count},
          _length{length}
    {
        init();
    }

    void update(const std::string& key, const std::string& data);
    bool compare(const std::string& key, const std::string& data) const;
    const std::string& getRandomKey() const;
    std::string getData(const std::string& key) const;

private:

    void init();

    const std::size_t _count;
    const std::size_t _length;

    std::unordered_map<std::string, Item> _storage;
    std::vector<std::string> _keys;
};
