#include <gtest/gtest.h>


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <utility>
#include <vector>
#include <memory>
#include <locale>


#include "generate_file.h"


const std::size_t read_workers = 3;
const std::size_t write_workers = 1;


struct Result
{
    std::size_t count = 0;
    std::chrono::nanoseconds read_io_time;
    std::chrono::nanoseconds total_io_time;
};

struct Worker
{
    std::thread thread;
    Result result;
};


void read_worker(std::atomic_bool*, Result*, const std::string&);
void write_worker(std::atomic_bool*, Result*, const std::string&);


struct separated : std::numpunct<char>
{
    virtual std::string do_grouping() const {
        return "\03";
    }
};

TEST(simple, simple) {

    std::locale loc{std::cout.getloc(), new separated};
    std::cout.imbue(loc);
    std::cout << "Run" << std::endl;

    generate_file("data.dat", 8 * 8 * 1024);

    std::atomic_bool is_stop{false};
    std::vector< std::unique_ptr<Worker> > workers;
    for (int i = 1; i <= read_workers + write_workers; i++) {

        auto w = std::make_unique<Worker>();

        if (i != read_workers + write_workers) {
            w->thread = std::thread{read_worker, &is_stop, &(w->result), "data.dat"};
        } else {
            w->thread = std::thread{write_worker, &is_stop, &(w->result), "data.dat"};
        }

        workers.push_back(std::move(w));
    }


    std::this_thread::sleep_for(std::chrono::seconds{10});

    is_stop = true;
    for (auto& w : workers) {
        w->thread.join();
        std::cout << "Iterations count: " << w->result.count \
                  << ", read IO time: " << w->result.read_io_time.count() \
                  << ", total IO time: " << w->result.total_io_time.count() \
                  << std::endl;
    }
}


void read_worker(std::atomic_bool* is_stop, Result* result, const std::string& filename) {

    const std::size_t len = 1024;
    auto buffer = std::make_unique<char[]>(len);

    while(!(*is_stop)) {

        auto start_time = std::chrono::high_resolution_clock::now();
        std::ifstream f{filename};

        while (f) {

            auto start_time = std::chrono::high_resolution_clock::now();
            f.read(buffer.get(), len);
            auto end_time = std::chrono::high_resolution_clock::now();
            result->read_io_time += end_time - start_time;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        result->total_io_time += end_time - start_time;

        result->count++;
    }

}

void write_worker(std::atomic_bool* is_stop, Result* result, const std::string&) {

    while(!(*is_stop)) {
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
        result->count++;
    }

}
