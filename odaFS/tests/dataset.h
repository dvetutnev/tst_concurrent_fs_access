#pragma once


#include <fs/common.h>

#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>
#include <memory>


void generateData(char* buffer, std::size_t length);


class DataSet
{
public:

    struct Item
    {
        std::string data;
        mutable std::mutex topMtx;
        mutable std::shared_mutex bottomMtx;
    };

    class SharedLock
    {
    public:

        SharedLock(const Item& item);
        ~SharedLock();

    private:

        const Item& _item;
    };


    class UniqueLock
    {
    public:

        UniqueLock(const Item& item);
        ~UniqueLock();

    private:

        const Item& _item;
    };

    DataSet(std::size_t count, std::size_t length = 8 * 1024)
        : _count{count},
          _length{length}
    {
        init();
    }

    std::unique_ptr<UniqueLock> update(const oda::fs::Path&, const std::string&);
    bool compare(const oda::fs::Path&, const std::string&) const;
    const oda::fs::Path& getRandomPath() const;
    std::string getData(const oda::fs::Path&) const;
    const std::vector<oda::fs::Path>& getAllPaths() const;

private:

    void init();

    const std::size_t _count;
    const std::size_t _length;

    std::unordered_map<oda::fs::Path::string_type, Item> _storage;
    std::vector<oda::fs::Path> _paths;
};


class DataSetFiles
{
public:

    DataSetFiles(const DataSet& dataSet)
        : _dataSet{dataSet}
    {
        init();
    }

    ~DataSetFiles();

private:

    void init();

    const DataSet& _dataSet;
};
