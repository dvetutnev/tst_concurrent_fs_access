#include "worker.h"

#include <easy/profiler.h>

#include <algorithm>
#include <utility>


void BaseWorker::start() {

    _isRun = true;
    _thread = std::thread{&BaseWorker::worker, this};
}

void BaseWorker::stop() {

    _isRun = false;
    _thread.join();
}

void BaseWorker::worker() {

    EASY_THREAD(getClassName().c_str());

    while (_isRun) {

        _count++;
        handler();
    }
}

void BaseWorker::printResult() const {


    std::cout << getClassName() << ", iterations count: " << _count << std::endl;
}


std::string groupingChunk(const std::vector<Chunk>& chunks) {

    EASY_FUNCTION(::profiler::colors::Silver, ::profiler::OFF);

    std::size_t totalLength = 0;
    for (const auto& i : chunks) {

        totalLength += i.length;
    }

    std::string ret;
    ret.resize(totalLength);

    auto it = std::begin(ret);
    for (const auto& i : chunks) {

        it = std::copy_n(i.data.get(), i.length, it);
    }

    return std::move(ret);
}
