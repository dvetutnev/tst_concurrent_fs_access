#include "data_set.h"

#include <gtest/gtest.h>
#include <easy/profiler.h>

#include <atomic>
#include <thread>
#include <chrono>


TEST(DataSet, getRandomKey) {

    DataSet dataSet{3};

    const std::string& key = dataSet.getRandomKey();

    std::cout << "dataSet.getRandomKey(): " << key << std::endl;
    ASSERT_TRUE(key == "data_1.dat" || key == "data_2.dat" || key == "data_3.dat");
}


TEST(DataSet, getRandomKey_10k) {

    EASY_MAIN_THREAD;
    EASY_PROFILER_ENABLE;

    DataSet dataSet{10000};
    const std::string& key = dataSet.getRandomKey();
    std::cout << "dataSet.getRandomKey(): " << key << std::endl;

    EASY_PROFILER_DISABLE;
    auto blocks_count = ::profiler::dumpBlocksToFile("DataSet_getRandomKey_10k.prof");
    std::cout << "Blocks count: " << blocks_count << std::endl;
}


TEST(DataSet, getData) {

    DataSet dataSet{3};

    ASSERT_NO_THROW(dataSet.getData("data_1.dat"));
    ASSERT_NO_THROW(dataSet.getData("data_2.dat"));
    ASSERT_NO_THROW(dataSet.getData("data_3.dat"));
    ASSERT_ANY_THROW(dataSet.getData("data_4.dat"));
}


TEST(DataSet, update) {

    DataSet dataSet{3};

    const std::string newData = "AAAAAAaaaaaaBBBBBBBbbbbbCCCCCccc";
    ASSERT_NO_THROW(dataSet.update("data_1.dat", newData));
    ASSERT_NO_THROW(dataSet.update("data_2.dat", newData));
    ASSERT_NO_THROW(dataSet.update("data_3.dat", newData));
    ASSERT_ANY_THROW(dataSet.update("data_4.dat", newData));

    const std::string result = dataSet.getData("data_2.dat");
    ASSERT_EQ(result, newData);
}


TEST(DataSet, compare) {

    DataSet dataSet{3};

    ASSERT_NO_THROW(dataSet.compare("data_1.dat", ""));
    ASSERT_NO_THROW(dataSet.compare("data_2.dat", ""));
    ASSERT_NO_THROW(dataSet.compare("data_3.dat", ""));
    ASSERT_ANY_THROW(dataSet.compare("data_4.dat", ""));

    const std::string badData = "badData";
    const std::string normalData = dataSet.getData("data_2.dat");
    ASSERT_FALSE(dataSet.compare("data_2.dat", badData));
    ASSERT_TRUE(dataSet.compare("data_2.dat", normalData));
}


namespace {


class ReadWorker
{
public:

    ReadWorker(DataSet& dataSet)
        : _dataSet{dataSet},
          _data{"vmsbtrjhdrnnkHKlklHLz;lsjv;zlsnv;znlc;in;asc"}
    {}

    void start() {

        _count = 0;
        _isRun = true;
        _thread = std::thread{&ReadWorker::worker, this};
    }

    void stop() {

        _isRun = false;
        _thread.join();
        std::cout << "ReadWorker, iterations count: " << _count << std::endl;
    }

private:

    void worker() {

        while (_isRun) {

            const std::string& key = _dataSet.getRandomKey();
            _dataSet.compare(key, _data);

            _count++;
        }
    }

    DataSet& _dataSet;
    const std::string _data;

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};


class WriteWorker
{
public:

    WriteWorker(DataSet& dataSet)
        : _dataSet{dataSet},
          _data{"vmsbtrjhdrnnkHKlklHLz;lsjv;zlsnv;znlc;in;asc"}
    {}

    void start() {

        _count = 0;
        _isRun = true;
        _thread = std::thread{&WriteWorker::worker, this};
    }

    void stop() {

        _isRun = false;
        _thread.join();
        std::cout << "WriteWorker, iterations count: " << _count << std::endl;
    }

private:

    void worker() {

        while (_isRun) {

            const std::string& key = _dataSet.getRandomKey();
            _dataSet.update(key, _data);

            _count++;
        }
    }

    DataSet& _dataSet;
    const std::string _data;

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};

}   // anonimus namespace


TEST(DataSet, concurrentAccess_1) {

    DataSet dataSet{1};


    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    WriteWorker writeWorker{dataSet};


    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    writeWorker.start();


    std::this_thread::sleep_for(std::chrono::seconds{1});


    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();

    writeWorker.stop();


    SUCCEED();
}


TEST(DataSet, concurrentAccess_2) {

    DataSet dataSet{2};


    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    WriteWorker writeWorker{dataSet};


    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    writeWorker.start();


    std::this_thread::sleep_for(std::chrono::seconds{1});


    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();

    writeWorker.stop();


    SUCCEED();
}


TEST(DataSet, concurrentAccess_3) {

    DataSet dataSet{3};


    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    WriteWorker writeWorker{dataSet};


    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    writeWorker.start();


    std::this_thread::sleep_for(std::chrono::seconds{1});


    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();

    writeWorker.stop();


    SUCCEED();
}


TEST(DataSet, concurrentAccess_10) {

    DataSet dataSet{10000};


    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    WriteWorker writeWorker{dataSet};


    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    writeWorker.start();


    std::this_thread::sleep_for(std::chrono::seconds{1});


    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();

    writeWorker.stop();


    SUCCEED();
}
