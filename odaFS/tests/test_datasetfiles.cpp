#include "dataset.h"


#include <gtest/gtest.h>
#include <boost/filesystem.hpp>


TEST(DataSetFiles, create) {

    DataSet dataSet{3};
    DataSetFiles dataSetFiles{dataSet};

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        ASSERT_TRUE(boost::filesystem::is_regular_file(path));
    }
}


TEST(DataSetFiles, content) {

    DataSet dataSet{3};
    DataSetFiles dataSetFiles{dataSet};

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        const std::string normalResult = dataSet.getData(path);
        std::string result;
        result.reserve(normalResult.length());

        boost::filesystem::ifstream file{path, std::ios_base::binary};
        while (!file.eof()) {

            std::string buffer;
            buffer.resize(1024);

            file.read(&buffer[0], 1024);
            const std::size_t count = file.gcount();
            buffer.resize(count);

            result += buffer;
        }

        ASSERT_EQ(result.length(), normalResult.length());
        ASSERT_EQ(result, normalResult);
    }
}


TEST(DataSetFiles, remove) {

        DataSet dataSet{3};
    {
        DataSetFiles dataSetFiles{dataSet};
    }

    const auto& paths = dataSet.getAllPaths();
    for(const auto& path : paths) {

        ASSERT_FALSE(boost::filesystem::exists(path));
    }
}