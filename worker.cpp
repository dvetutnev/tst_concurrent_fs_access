#include "worker.h"


#include <iostream>


void BaseWorker::start() {

    _isRun = true;
    _thread = std::thread{&BaseWorker::worker, this};
}

void BaseWorker::stop() {

    _isRun = false;
    _thread.join();
}

void BaseWorker::worker() {

    while (_isRun) {

        _count++;
        handler();
    }
}

void BaseWorker::printResult() const {

    std::cout << name() << ", iterations count: " << _count << std::endl;
}
