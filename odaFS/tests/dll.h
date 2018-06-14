#pragma once

#ifdef _WIN32
    #ifdef COMPILING_THE_DLL
        #define MY_DLL_EXPORT extern "C" __declspec(dllexport)
    #else
        #define MY_DLL_EXPORT extern "C" __declspec(dllimport)
    #endif
#else
    #define MY_DLL_EXPORT
#endif

namespace dll {

MY_DLL_EXPORT void printAddress();

}
