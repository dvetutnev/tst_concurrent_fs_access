#include "dll.h"
#include <fs/fs.h>

#include <iostream>


int main(int, char**) {

    std::cout << "From main address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLLprint();

    oda::fs::internal::insertDummyLock();

    std::cout << "From main address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLLprint();
}
