#pragma once


#include <atomic>
#include <thread>


class BaseWorker
{
public:

    BaseWorker()
        : _count{0}
    {}
    void start();
    void stop();
    void printResult() const;
    virtual ~BaseWorker() = default;

protected:

    virtual void handler() = 0;
    virtual const char* name() const = 0;

private:

    void worker();

    std::size_t _count;
    std::atomic_bool _isRun;
    std::thread _thread;
};
