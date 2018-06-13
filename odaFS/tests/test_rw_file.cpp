#include <fs/file.h>
#include "dataset.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>


namespace {


class ReadWorker
{
public:

    ReadWorker(const DataSet& dataSet)
        : _dataSet{dataSet}
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
            const std::string content = oda::fs::readFile(path);

            _count++;
        }
    }

    const DataSet& _dataSet;

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};


} // anonimous namespace


TEST(rwFile, readOnly_1) {

    DataSet dataSet{1};
    DataSetFiles files{dataSet};

    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();
}


TEST(rwFile, readOnly_3) {

    DataSet dataSet{3};
    DataSetFiles files{dataSet};

    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();
}


TEST(rwFile, readOnly_10k) {

    DataSet dataSet{10000};
    DataSetFiles files{dataSet};

    ReadWorker readWorker_1{dataSet};
    ReadWorker readWorker_2{dataSet};
    ReadWorker readWorker_3{dataSet};
    ReadWorker readWorker_4{dataSet};

    readWorker_1.start();
    readWorker_2.start();
    readWorker_3.start();
    readWorker_4.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    readWorker_1.stop();
    readWorker_2.stop();
    readWorker_3.stop();
    readWorker_4.stop();
}


#ifndef ODA_FS_FAKE_LOCK

namespace {


class WriteWorker
{
public:

    WriteWorker(DataSet& dataSet)
        : _dataSet{dataSet},
          _length{_dataSet.getData(_dataSet.getRandomPath()).length()}
    {}

    void start() {

        _count = 0;
        _isRun = true;
        _thread = std::thread{&WriteWorker::worker, this};
    }

    void stop() {

        _isRun = false;
        _thread.join();
        std::cout << "ReadWorker, iterations count: " << _count << std::endl;
    }

private:

    void worker() {

        std::string data;
        data.resize(_length);

        while (_isRun) {

            generateData(&data[0], data.length());

            const oda::fs::Path& path = _dataSet.getRandomPath();
            _dataSet.update(path, data);

            oda::fs::writeFile(path, data, std::ios_base::trunc);

            _count++;
        }
    }

    DataSet& _dataSet;
    const std::size_t _length;

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};


} // anonimous namespace


TEST(rwFile, rw_1) {

    DataSet dataSet{1};
    DataSetFiles files{dataSet};

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
}


TEST(rwFile, rw_3) {

    DataSet dataSet{3};
    DataSetFiles files{dataSet};

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
}


TEST(rwFile, rw_10k) {

    DataSet dataSet{10000};
    DataSetFiles files{dataSet};

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
}

#endif
