#include <fs/fs.h>
#include "dataset.h"

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <cassert>


namespace {


std::string _readFile(const oda::fs::Path& path) {

    std::string result;

    boost::filesystem::ifstream file{path, std::ios_base::binary};
    while (file) {

        std::string buffer;
        buffer.resize(4096);

        file.read(&buffer[0], 4096);
        const auto count = file.gcount();
        assert(count >= 0);
        buffer.resize(static_cast<std::size_t>(count));

        result += buffer;
    }

    return result;
}


} // anonymous namespace


TEST(writeFile, truncate_create) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "write_file";

    if(boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    std::string normalResult;
    normalResult.resize(1234);
    generateData(&normalResult[0], normalResult.length());

    oda::fs::writeFile(path, normalResult, std::ios_base::trunc);

    const std::string result = _readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(writeFile, truncate_override) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "write_file";

    if(boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    {
        std::string data;
        data.resize(1235);
        generateData(&data[0], data.length());
        boost::filesystem::ofstream file{path, std::ios_base::binary};
        file << data;
    }

    std::string normalResult;
    normalResult.resize(1234);
    generateData(&normalResult[0], normalResult.length());

    oda::fs::writeFile(path, normalResult, std::ios_base::trunc);

    const std::string result = _readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(writeFile, append) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "write_file";

    if(boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    std::string head;
    head.resize(1234);
    generateData(&head[0], head.length());
    boost::filesystem::ofstream file{path, std::ios_base::binary};
    file << head;
    file.close();

    std::string tail;
    tail.resize(1234);
    generateData(&tail[0], tail.length());

    oda::fs::writeFile(path, tail, std::ios_base::app);

    const std::string result = _readFile(path);

    ASSERT_EQ(result.length(), head.length() + tail.length());
    ASSERT_EQ(result, head + tail);
}


TEST(writeFile, large) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "write_file";

    if(boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    std::string normalResult;
    normalResult.resize(500 * 1024 * 1024);
    generateData(&normalResult[0], normalResult.length());

    oda::fs::writeFile(path, normalResult, std::ios_base::trunc);

    const std::string result = _readFile(path);

    ASSERT_EQ(result.length(), normalResult.length());
    ASSERT_EQ(result, normalResult);
}


TEST(writeFile, is_directory) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "write_file_is_directory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    boost::filesystem::create_directory(path);

    try {

        std::string data;
        data.resize(1234);
        oda::fs::writeFile(path, data, std::ios_base::trunc);
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


TEST(writeFile, access_denied) {

#ifdef _WIN32
    const oda::fs::Path path = "C:\\System Volume Information\\write_file_access_denied";
#else
    const oda::fs::Path path = "/root/write_file_access_denied";
#endif

    try {

        std::string data;
        data.resize(1234);
        oda::fs::writeFile(path, data, std::ios_base::trunc);
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
