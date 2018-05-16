#include "data_set.h"

#include <gtest/gtest.h>
#include <easy/profiler.h>


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

