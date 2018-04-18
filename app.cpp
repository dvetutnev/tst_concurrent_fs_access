#include <gtest/gtest.h>


#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <utility>
#include <vector>
#include <memory>


#include <shared_mutex>


#include "generate_file.h"
#include "fs_lock.h"


class Worker
{
public:

    Worker()
        : _count{0},
          _ioTime{0},
          _ioTimeTotal{0}
    {}

    void start() {

        _isRun = true;
        _thread = std::thread{&Worker::handler, this};
    }

    void stop() {

        _isRun = false;
        _thread.join();
    }

    void printResult() const {

        std::cout << name() \
                  << ", iterations count: " << _count \
                  << ", read IO time: " << _ioTime.count() \
                  << ", total IO time: " << _ioTimeTotal.count() \
                  << std::endl;
    }

    virtual ~Worker() = default;

protected:

    virtual std::string name() const = 0;
    virtual void worker() = 0;

    std::size_t _count;
    std::chrono::nanoseconds _ioTime;
    std::chrono::nanoseconds _ioTimeTotal;

private:

    void handler() {

        while (_isRun) {
            _count++;
            worker();
        }
    }

    std::atomic_bool _isRun;
    std::thread _thread;
};


template<typename FS>
class ReadWorker : public Worker
{
public:

    ReadWorker(const std::string& filepath)
        : _filepath{filepath},
          _buffer{std::make_unique<char[]>(1024)}
    {}

protected:

    virtual std::string name() const override {

        return "ReadWorker";
    }

    virtual void worker() override {

        auto startTime = std::chrono::high_resolution_clock::now();
        typename FS::shared_lock l{_filepath};
        std::ifstream f{_filepath};

        while (f) {

            auto startTime = std::chrono::high_resolution_clock::now();
            f.read(_buffer.get(), 1024);
            auto endTime = std::chrono::high_resolution_clock::now();
            _ioTime += endTime - startTime;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        _ioTimeTotal += endTime - startTime;
    }

private:

    const std::string _filepath;
    std::unique_ptr<char[]> _buffer;
};


template<typename FS>
class WriteWorker : public Worker
{
public:

    WriteWorker(const std::string& filepath)
        : _filepath{filepath},
          _buffer{std::make_unique<char[]>(8 * 1024)}
    {}

protected:

    virtual std::string name() const override {

        return "WriteWorker";
    }

    virtual void worker() override {

        auto startTime = std::chrono::high_resolution_clock::now();
        typename FS::unique_lock l{_filepath};
        std::ofstream f{_filepath};

        {
            auto startTime = std::chrono::high_resolution_clock::now();
            f.write(_buffer.get(), 1024);
            auto endTime = std::chrono::high_resolution_clock::now();
            _ioTime += endTime - startTime;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        _ioTimeTotal += endTime - startTime;
    }

private:

    const std::string _filepath;
    std::unique_ptr<char[]> _buffer;
};


struct FakeLock
{
    struct shared_lock
    {
        shared_lock(const std::string&) {}
    };


    struct unique_lock
    {
        unique_lock(const std::string&) {}
    };
};

TEST(FakeLock, readOnly) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<FakeLock> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(TestSharedMtx, simple) {

    std::shared_mutex mtx;

    std::thread shared_1{
        [&]() {
            mtx.lock_shared();
            std::cout << "shared_1 start" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1000});
            std::cout << "shared_1 end" << std::endl;
            mtx.unlock_shared();
        }
    };

    std::thread shared_2{
        [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            mtx.lock_shared();
            std::cout << "shared_2 start" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1000});
            std::cout << "shared_2 end" << std::endl;
            mtx.unlock_shared();
        }
    };

    std::thread unique{
        [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds{200});
            mtx.lock();
            std::cout << "unique start" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1000});
            std::cout << "unique end" << std::endl;
            mtx.unlock();
        }
    };

    std::thread shared_3{
        [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds{400});
            mtx.lock_shared();
            std::cout << "shared_3 start" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1000});
            std::cout << "shared_3 end" << std::endl;
            mtx.unlock_shared();
        }
    };

    shared_1.join();
    shared_2.join();
    unique.join();
    shared_3.join();
}


TEST(SimpleLock_v_0_0, readOnly) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<Lock_v_0_0> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(SimpleLock_v_0_1, readOnly) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<Lock_v_0_1> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(SimpleLock_v_0_1, readWrite) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<Lock_v_0_1> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }
    auto w = std::make_unique< WriteWorker<Lock_v_0_1> >("data.dat");
    w->start();
    workers.push_back(std::move(w));

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(SimpleLock_v_0_2, readOnly) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<Lock_v_0_1> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(SimpleLock_v_0_2, readWrite) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<Lock_v_0_2> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }
    auto w = std::make_unique< WriteWorker<Lock_v_0_2> >("data.dat");
    w->start();
    workers.push_back(std::move(w));

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(TwoLevelLock_v_1_0, readOnly) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<TwoLevelLock_v_1_0> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}


TEST(TwoLevelLock_v_1_0, readWrite) {

    generate_file("data.dat", 8 * 8 * 1024);

    std::vector< std::unique_ptr<Worker> > workers;
    while (workers.size() < 4) {
        auto w = std::make_unique< ReadWorker<TwoLevelLock_v_1_0> >("data.dat");
        w->start();
        workers.push_back(std::move(w));
    }
    auto w = std::make_unique< WriteWorker<TwoLevelLock_v_1_0> >("data.dat");
    w->start();
    workers.push_back(std::move(w));

    std::this_thread::sleep_for(std::chrono::seconds{10});

    for (auto& i : workers) {
        i->stop();
        i->printResult();
    }
}
