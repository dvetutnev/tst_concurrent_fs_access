#include "dll.h"
#include <fs/fs.h>

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


int main(int, char**) {

    std::cout << "From main address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;

    using FuncPtr = void (*)();
    const char* funcName = "DLLprint";

#ifdef _WIN32
    HMODULE handle = ::LoadLibraryW(L"./dll.dll");
    if (handle == nullptr) {
        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot open library, Win32 code: " << errorCode << std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto funcPtr = reinterpret_cast<FuncPtr>(::GetProcAddress(handle, funcName));
    if (funcPtr == nullptr) {
        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot load sysmbol '" << funcName << "', Win32 code: " << errorCode << std::endl;
        std::exit(EXIT_FAILURE);
    }

    funcPtr();

    oda::fs::internal::insertDummyLock();

    std::cout << "From main address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    funcPtr();

    ::FreeLibrary(handle);
#else
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
#endif
    std::exit(EXIT_SUCCESS);
}
