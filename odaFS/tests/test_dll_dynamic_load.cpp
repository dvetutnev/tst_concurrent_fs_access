/*
 * ODA filesystem. Test for unique internal object.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "dll.h"
#include <fs/fs.h>

#include <gtest/gtest.h>

#include <cstdlib>
#include <exception>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


TEST(testDLL_dynamic_load, fake) {

    SUCCEED();
}


#ifdef ODA_FS_TESTS

struct DLLFunctions
{
#ifdef _WIN32
    HMODULE libraryHandle;
#else
    void* libraryHandle;
#endif

    using printPtr = void (*)();
    printPtr print;

    using addressOfLocksPtr = void* (*)();
    addressOfLocksPtr addressOfLocks;

    using sizeOfLocksPtr = std::size_t (*)();
    sizeOfLocksPtr sizeOfLocks;

    using insertDummyLockPtr = void (*)();
    insertDummyLockPtr insertDummyLock;
};

DLLFunctions loadFunctions() {

    DLLFunctions result;

#ifdef _WIN32

    HMODULE handle = ::LoadLibraryW(L"./dll.dll");
    if (handle == nullptr) {

        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot open library, Win32 code: " << errorCode << std::endl;
        throw std::runtime_error{"loadFunctions"};
    }
    result.libraryHandle = handle;

    auto print = reinterpret_cast<DLLFunctions::printPtr>(::GetProcAddress(handle, "DLLprint"));
    if (print == nullptr) {

        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot load sysmbol 'DLLprint', Win32 code: " << errorCode << std::endl;
        throw std::runtime_error{"loadFunctions"};
    }
    result.print = print;

    auto addressOfLocks = reinterpret_cast<DLLFunctions::addressOfLocksPtr>(::GetProcAddress(handle, "DLLaddressOfLocks"));
    if (print == nullptr) {

        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot load sysmbol 'DLLaddressOfLocks', Win32 code: " << errorCode << std::endl;
        throw std::runtime_error{"loadFunctions"};
    }
    result.addressOfLocks = addressOfLocks;

    auto sizeOfLocks = reinterpret_cast<DLLFunctions::sizeOfLocksPtr>(::GetProcAddress(handle, "DLLsizeOfLocks"));
    if (print == nullptr) {

        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot load sysmbol 'DLLsizeOfLocks', Win32 code: " << errorCode << std::endl;
        throw std::runtime_error{"loadFunctions"};
    }
    result.sizeOfLocks = sizeOfLocks;

    auto insertDummyLock = reinterpret_cast<DLLFunctions::insertDummyLockPtr>(::GetProcAddress(handle, "DLLinsertDummyLock"));
    if (print == nullptr) {

        const auto errorCode = ::GetLastError();
        std::cerr << "Cannot load sysmbol 'DLLinsertDummyLock', Win32 code: " << errorCode << std::endl;
        throw std::runtime_error{"loadFunctions"};
    }
    result.insertDummyLock = insertDummyLock;

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

    return result;
}

void unLoadFunctions(const DLLFunctions& DLL) {
#ifdef _WIN32
    ::FreeLibrary(DLL.libraryHandle);
#else
    dlclose(DLL.libraryHandle);
#endif
}


TEST(testDLL_dynamic_load, address_and_size__insert_from_EXE) {

    DLLFunctions DLL = loadFunctions();

    void* const addressFromEXE = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL = DLL.addressOfLocks();

    const std::size_t sizeFromEXE = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL = DLL.sizeOfLocks();

    ASSERT_EQ(addressFromEXE, addressFromDLL);
    ASSERT_EQ(sizeFromEXE, sizeFromDLL);


    oda::fs::internal::insertDummyLock();


    void* const addressFromEXE_after_insert = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL_after_insert = DLL.addressOfLocks();

    const std::size_t sizeFromEXE_after_insert = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL_after_insert = DLL.sizeOfLocks();

    ASSERT_EQ(addressFromEXE_after_insert, addressFromDLL_after_insert);
    ASSERT_EQ(sizeFromEXE_after_insert, sizeFromDLL_after_insert);

    unLoadFunctions(DLL);
    SUCCEED();
}


TEST(testDLL_dynamic_load, address_and_size__insert_from_DLL) {

    DLLFunctions DLL = loadFunctions();

    void* const addressFromEXE = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL = DLL.addressOfLocks();

    const std::size_t sizeFromEXE = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL = DLL.sizeOfLocks();

    ASSERT_EQ(addressFromEXE, addressFromDLL);
    ASSERT_EQ(sizeFromEXE, sizeFromDLL);


    DLL.insertDummyLock();


    void* const addressFromEXE_after_insert = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL_after_insert = DLL.addressOfLocks();

    const std::size_t sizeFromEXE_after_insert = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL_after_insert = DLL.sizeOfLocks();

    ASSERT_EQ(addressFromEXE_after_insert, addressFromDLL_after_insert);
    ASSERT_EQ(sizeFromEXE_after_insert, sizeFromDLL_after_insert);

    unLoadFunctions(DLL);
    SUCCEED();
}


TEST(testDLL_dynamic_load, print) {

    DLLFunctions DLL = loadFunctions();

    std::cout << "From EXE address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLL.print();

    std::cout << "Insert dummy lock..." << std::endl;
    oda::fs::internal::insertDummyLock();

    std::cout << "From EXE address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLL.print();

    unLoadFunctions(DLL);
    SUCCEED();
}

#endif // ODA_FS_TESTS
