project(tst_concurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


include(conan.cmake)
conan_cmake_run(
    REQUIRES
        googletest/1.9.0-b2@odant/stable
        easy_profiler/2.0.0@odant/stable
        boost/1.67.0@odant/testing
    BASIC_SETUP
    CMAKE_TARGETS
    NO_OUTPUT_DIRS
    UPDATE
    BUILD
        outdated
    IMPORTS
        "bin, *.dll -> ./bin"
        "bin, *.pdb -> ./bin"
        "lib, *.so* -> ./bin"
)



if(MSVC)
    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
    )
endif()

enable_testing()

include_directories(include)
add_subdirectory(odaFS)
