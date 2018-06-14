#include "dll.h"
#include <fs/fs.h>

#include <iostream>


int main(int, char**) {

    std::cout << "From main: " << oda::fs::internal::addressOfLocks() << std::endl;
    dll::printAddress();
}
