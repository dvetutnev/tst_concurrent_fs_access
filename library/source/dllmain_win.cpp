#include <Windows.h>

#include <boost/locale/generator.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>


extern "C" {

BOOL WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:

        {   // Setup default locale
            const boost::locale::generator gen;
            const std::locale loc = gen.generate(std::locale(), "");
            std::locale::global(loc);
            std::cout.imbue(loc);
            std::wcout.imbue(loc);
            boost::filesystem::path::imbue(loc);
        }
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

} // extern "C"
