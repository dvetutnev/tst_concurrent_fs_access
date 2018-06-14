#include "dll.h"
#include <fs/fs.h>

#include <iostream>


namespace dll {

void printAddress() {

    std::cout << "From dll: " << oda::fs::internal::addressOfLocks() << std::endl;
}

}
