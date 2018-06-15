/*
 * ODA filesystem. Test for unique internal object.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "dll.h"
#include <fs/fs.h>

#include <iostream>


void DLLprint() {

    std::cout << "From DLL address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
}


void* DLLaddressOfLocks() {

    return oda::fs::internal::addressOfLocks();
}


std::size_t DLLsizeOfLocks() {

    return oda::fs::internal::sizeOfLocks();
}


void DLLinsertDummyLock() {

    oda::fs::internal::insertDummyLock();
}
