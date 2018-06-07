#include <fs/file.h>
#include <fs/directory.h>
#include "dataset.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>


TEST(readFile, simple) {

    DataSet dataSet{3};
    DataSetFiles files{dataSet};

    const oda::fs::Path& path = dataSet.getRandomPath();
    const std::string normalResult = dataSet.getData(path);
    const std::string result = oda::fs::readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(readFile, not_exists) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "read_file_not_exists";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    try {

        oda::fs::readFile(path);
        FAIL() << "No throw exception!";

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }

    SUCCEED();
}


TEST(readFile, is_directory) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "read_file_is_directory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    boost::filesystem::create_directory(path);

    try {

        oda::fs::readFile(path);
        FAIL() << "No throw exception!";

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }

    SUCCEED();
}


TEST(readFile, access_denied) {

#ifdef _WIN32
    const oda::fs::Path path = "C:\\Windows\\System32\\config\\SAM";
#else
    const oda::fs::Path path = "/root/.profile";
#endif

    try {

        oda::fs::readFile(path);
        FAIL() << "No throw exception!";

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }

    SUCCEED();
}


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

/*
TEST(IOFiles, readOnly_1) {

    DataSet dataSet{1};
    DataSetFiles files{dataSet};

    ReadWorker readWorker_1{dataSet};

    readWorker_1.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    readWorker_1.stop();
}
*/
