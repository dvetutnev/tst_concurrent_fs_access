#include "dataset.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>


TEST(DataSet, getRandomPath) {

    DataSet dataSet{".", 3};

    const auto& path = dataSet.getRandomPath();

    std::cout << "dataSet.getRandomKey(): " << path << std::endl;
    ASSERT_TRUE(
                path == oda::fs::Path{"./data_1.dat"}
                || path == oda::fs::Path{"./data_2.dat"}
                || path == oda::fs::Path{"./data_3.dat"}
                );
}


TEST(DataSet, getAllPaths) {

    DataSet dataSet{".", 3};

    const auto& paths = dataSet.getAllPaths();

    ASSERT_EQ(paths.size(), 3);
    ASSERT_EQ(paths[0], "./data_1.dat");
    ASSERT_EQ(paths[1], "./data_2.dat");
    ASSERT_EQ(paths[2], "./data_3.dat");
}


TEST(DataSet, getData) {

    DataSet dataSet{".", 3};
#ifdef _WIN32
    ASSERT_NO_THROW(dataSet.getData(".\\data_1.dat"));
    ASSERT_NO_THROW(dataSet.getData(".\\data_2.dat"));
    ASSERT_NO_THROW(dataSet.getData(".\\data_3.dat"));
    ASSERT_ANY_THROW(dataSet.getData(".\\data_4.dat"));
#else
    ASSERT_NO_THROW(dataSet.getData("./data_1.dat"));
    ASSERT_NO_THROW(dataSet.getData("./data_2.dat"));
    ASSERT_NO_THROW(dataSet.getData("./data_3.dat"));
    ASSERT_ANY_THROW(dataSet.getData("./data_4.dat"));
#endif
}


TEST(DataSet, update) {

    DataSet dataSet{".", 3};

    const std::string newData = "AAAAAAaaaaaaBBBBBBBbbbbbCCCCCccc";
#ifdef _WIN32
    ASSERT_NO_THROW(dataSet.update(".\\data_1.dat", newData));
    ASSERT_NO_THROW(dataSet.update(".\\data_2.dat", newData));
    ASSERT_NO_THROW(dataSet.update(".\\data_3.dat", newData));
    ASSERT_ANY_THROW(dataSet.update(".\\data_4.dat", newData));
#else
    ASSERT_NO_THROW(dataSet.update("./data_1.dat", newData));
    ASSERT_NO_THROW(dataSet.update("./data_2.dat", newData));
    ASSERT_NO_THROW(dataSet.update("./data_3.dat", newData));
    ASSERT_ANY_THROW(dataSet.update("./data_4.dat", newData));
#endif

#ifdef _WIN32
    const std::string result = dataSet.getData(".\\data_2.dat");
#else
    const std::string result = dataSet.getData("./data_2.dat");
#endif
    ASSERT_EQ(result, newData);
}


TEST(DataSet, compare) {

    DataSet dataSet{".", 3};

#ifdef _WIN32
    ASSERT_NO_THROW(dataSet.compare(".\\data_1.dat", ""));
    ASSERT_NO_THROW(dataSet.compare(".\\data_2.dat", ""));
    ASSERT_NO_THROW(dataSet.compare(".\\data_3.dat", ""));
    ASSERT_ANY_THROW(dataSet.compare(".\\data_4.dat", ""));
#else
    ASSERT_NO_THROW(dataSet.compare("./data_1.dat", ""));
    ASSERT_NO_THROW(dataSet.compare("./data_2.dat", ""));
    ASSERT_NO_THROW(dataSet.compare("./data_3.dat", ""));
    ASSERT_ANY_THROW(dataSet.compare("./data_4.dat", ""));
#endif

    const std::string badData = "badData";
#ifdef _WIN32
    const std::string normalData = dataSet.getData(".\\data_2.dat");
    ASSERT_FALSE(dataSet.compare(".\\data_2.dat", badData));
    ASSERT_TRUE(dataSet.compare(".\\data_2.dat", normalData));
#else
    const std::string normalData = dataSet.getData("./data_2.dat");
    ASSERT_FALSE(dataSet.compare("./data_2.dat", badData));
    ASSERT_TRUE(dataSet.compare("./data_2.dat", normalData));
#endif
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

            const oda::fs::Path& path = _dataSet.getRandomPath();
            _dataSet.compare(path, _data);

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

            const oda::fs::Path& path = _dataSet.getRandomPath();
            _dataSet.update(path, _data);

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

    DataSet dataSet{".", 1};


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

    DataSet dataSet{".", 2};


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

    DataSet dataSet{".", 3};


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


TEST(DataSet, concurrentAccess_10k) {

    DataSet dataSet{".", 10000};


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
