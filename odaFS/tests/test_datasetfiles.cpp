#include "dataset.h"

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <cassert>


TEST(DataSetFiles, create) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    DataSet dataSet{prefixPath, 3};
    DataSetFiles dataSetFiles{dataSet};

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        ASSERT_TRUE(boost::filesystem::is_regular_file(path));
    }
}


TEST(DataSetFiles, content) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    DataSet dataSet{prefixPath, 3};
    DataSetFiles dataSetFiles{dataSet};

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        const std::string normalResult = dataSet.getData(path);
        std::string result;
        result.reserve(normalResult.length());

        boost::filesystem::ifstream file{path, std::ios_base::binary};
        while (file) {

            std::string buffer;
            buffer.resize(1024);

            file.read(&buffer[0], 1024);
            const auto count = file.gcount();
            assert(count >= 0);
            buffer.resize(static_cast<std::size_t>(count));

            result += buffer;
        }

        ASSERT_EQ(result.length(), normalResult.length());
        ASSERT_EQ(result, normalResult);
    }
}


TEST(DataSetFiles, remove) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    DataSet dataSet{prefixPath, 3};
    {
        DataSetFiles dataSetFiles{dataSet};
    }

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        ASSERT_FALSE(boost::filesystem::exists(path));
    }
}
