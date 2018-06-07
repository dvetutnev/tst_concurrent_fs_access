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

            if (!_dataSet.compare(path, content)) {

                throw std::runtime_error{"ReadWorker failed, content is not correct!"};
            }

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

    readWorker_1.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    readWorker_1.stop();
}
