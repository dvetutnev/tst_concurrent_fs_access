#include "dll.h"
#include <fs/fs.h>

#include <iostream>


void DLLprintAddress() {

    std::cout << "From dll: " << oda::fs::internal::addressOfLocks() << std::endl;
}
