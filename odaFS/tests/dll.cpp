#include "dll.h"
#include <fs/fs.h>

#include <iostream>


void DLLprint() {

    std::cout << "From dll address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
}
