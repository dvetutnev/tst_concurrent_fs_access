#pragma once


#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>


void generate_data(char* buffer, std::size_t length);


class DataSet
{
public:

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

    struct Item
    {
        std::string data;
        mutable std::mutex topMtx;
        mutable std::shared_mutex bottomMtx;
    };

    void init();

    const std::size_t _count;
    const std::size_t _length;

    std::unordered_map<std::string, Item> _storage;
    std::vector<std::string> _keys;
};
