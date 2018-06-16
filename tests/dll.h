/*
 * ODA filesystem. Test for unique internal object.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <cstddef>


#ifdef _WIN32
    #ifdef TEST_DLL_BUILD
        #define TEST_DLL_EXPORT extern "C" __declspec(dllexport)
    #else
        #define TEST_DLL_EXPORT extern "C" __declspec(dllimport)
    #endif
#else
    #define TEST_DLL_EXPORT extern "C"
#endif


TEST_DLL_EXPORT void DLLprint();


TEST_DLL_EXPORT void* DLLaddressOfLocks();
TEST_DLL_EXPORT std::size_t DLLsizeOfLocks();
TEST_DLL_EXPORT void DLLinsertDummyLock();
