#include "worker.h"
#include "generate.h"

#include <gtest/gtest.h>
#include <easy/profiler.h>

#include <algorithm>
#include <memory>
#include <fstream>
#include <thread>
#include <chrono>


TEST(test_worker, groupingChunk) {

    std::vector<Chunk> items;


    const std::string data_1 = "bgdeslhgHghlhbfklbwdl";
    Chunk chunk_1{
        std::make_unique<char[]>(1024),
        data_1.length()
    };
    std::copy_n(std::begin(data_1), chunk_1.length, chunk_1.data.get());
    items.push_back(std::move(chunk_1));

    const std::string data_2 = "xbcdfxbOOOGkjj;FJbhlDRTHVb,k m";
    Chunk chunk_2{
        std::make_unique<char[]>(1024),
        data_2.length()
    };
    std::copy_n(std::begin(data_2), chunk_2.length, chunk_2.data.get());
    items.push_back(std::move(chunk_2));

    const std::string data_3 = "hl;hjvgs;dh;lhklhkluHOLhHOI:";
    Chunk chunk_3{
        std::make_unique<char[]>(1024),
        data_3.length()
    };
    std::copy_n(std::begin(data_3), chunk_3.length, chunk_3.data.get());
    items.push_back(std::move(chunk_3));


    const std::string normalResult = data_1 + data_2 + data_3;
    const std::string result = groupingChunk(items);
    ASSERT_EQ(result, normalResult);
}


TEST(test_worker, ReadWorker) {

    TestData testData;

    for (int i = 1; i <= 10000; i++) {

        const std::size_t length = 8 * 1024;
        auto buffer = std::make_unique<char[]>(length);
        generate_data(buffer.get(), length);

        const std::string filePath = "data_" + std::to_string(i) + ".dat";

        std::ofstream file{filePath, std::ios_base::out | std::ios_base::binary};
        file.write(buffer.get(), length);
        file.flush();
        file.close();

        testData[filePath].data = std::string{buffer.get(), length};
    }


    EASY_PROFILER_ENABLE;
    EASY_MAIN_THREAD;


    ReadWorker worker{testData};
    worker.start();
    std::this_thread::sleep_for(std::chrono::seconds{1});
    worker.stop();
    worker.printResult();


    EASY_PROFILER_DISABLE;
    auto blocks_count = profiler::dumpBlocksToFile("ReadWorker.prof");
    std::cout << "Blocks count: " << blocks_count << std::endl;
}
