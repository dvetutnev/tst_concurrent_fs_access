#include <fs/fs.h>
#include "dataset.h"

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <atomic>
#include <chrono>
#include <thread>


TEST(readFile, simple) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    DataSet dataSet{prefixPath, 3};
    DataSetFiles files{dataSet};

    const oda::fs::Path& path = dataSet.getRandomPath();
    const std::string normalResult = dataSet.getData(path);
    const std::string result = oda::fs::readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(readFile, large_file) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    DataSet dataSet{prefixPath, 1, 500 * 1024 * 1024};
    DataSetFiles files{dataSet};

    const oda::fs::Path& path = dataSet.getRandomPath();
    const std::string normalResult = dataSet.getData(path);
    const std::string result = oda::fs::readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(readFile, not_exists) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    const oda::fs::Path path = prefixPath / "read_file_not_exists";

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

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    const oda::fs::Path path = prefixPath / "read_file_is_directory";

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
