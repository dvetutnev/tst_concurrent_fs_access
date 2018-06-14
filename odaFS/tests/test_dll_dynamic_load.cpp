#include "dll.h"
#include <fs/fs.h>

#include <iostream>
#include <cstdlib>
#include <dlfcn.h>


int main(int, char**) {

    std::cout << "From main: " << oda::fs::internal::addressOfLocks() << std::endl;

    using FuncPtr = void (*)();
    const char* funcName = "DLLprintAddress";

    void* handle = dlopen("./libdll.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    dlerror();

    auto funcPtr = reinterpret_cast<FuncPtr>(dlsym(handle, funcName));
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol '" << funcName << "': " << dlsym_error << std::endl;
        dlclose(handle);
        std::exit(EXIT_FAILURE);
    }

    funcPtr();

    dlclose(handle);
    std::exit(EXIT_SUCCESS);
}
