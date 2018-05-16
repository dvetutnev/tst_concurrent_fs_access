#pragma once

#include <easy/profiler.h>

#include <atomic>
#include <thread>
#include <shared_mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <exception>


struct Item
{
    std::string data;
    mutable std::shared_mutex mtx;
};
using TestData = std::map<std::string, Item>;


class BaseWorker
{
public:

    BaseWorker()
        : _count{0}
    {}

    void start();
    void stop();
    void printResult() const;

    virtual ~BaseWorker() = default;

protected:

    virtual void handler() = 0;
    virtual const std::string& getClassName() const = 0;

private:

    void worker();

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};


struct Chunk
{
    std::unique_ptr<char[]> data;
    std::size_t length;
};

std::string groupingChunk(const std::vector<Chunk>&);


class ReadWorker : public BaseWorker
{
public:

    ReadWorker(TestData& testData)
        : _className{"ReadWorker"},
          _testData{testData}
    {}

protected:

    virtual void handler() {

        const std::string& filePath = getFilePath();

        EASY_BLOCK("TotalReadIO");

        EASY_BLOCK("OpenFile")
        std::ifstream file{filePath, std::ios_base::in | std::ios_base::binary};
        if (!file) {
            throw std::exception{"Open file failed!"};
        }
        EASY_END_BLOCK;

        std::vector<Chunk> chunks;

        EASY_BLOCK("ReadIO");
        while (!file.eof()) {

            Chunk chunk{std::make_unique<char[]>(1024), 0};

            file.read(chunk.data.get(), 1024);
            chunk.length = file.gcount();

            chunks.push_back(std::move(chunk));
        }
        EASY_END_BLOCK;

        EASY_BLOCK("Check read data", profiler::colors::Silver);
        check(chunks, filePath);
    }

    virtual const std::string& getClassName() const override {

        return _className;
    }

private:

    const std::string& getFilePath() const {

        static std::string ret = "data_555.dat";
        return ret;
    }

    void check(const std::vector<Chunk>& chunks, const std::string& filePath) const {

        const std::string readResult = groupingChunk(chunks);

        EASY_BLOCK("Find item", profiler::colors::Silver);
        const auto& item = _testData[filePath];
        EASY_END_BLOCK;

        EASY_BLOCK("Get shared lock", profiler::colors::Silver);
        std::shared_lock<std::shared_mutex> lock{item.mtx};
        EASY_END_BLOCK;


        const std::string& normalResult = item.data;

        EASY_BLOCK("Compare data", profiler::colors::Silver);
        if (readResult != normalResult) {
            throw std::exception{"Readed data incorrect!"};
        }
        EASY_END_BLOCK;
    }

    const std::string _className;
    TestData& _testData;
};
