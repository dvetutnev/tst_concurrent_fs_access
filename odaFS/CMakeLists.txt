project(odaFS CXX)
cmake_minimum_required(VERSION 3.0)


option(ODAFS_TESTS "Enable odaFS unittests" ON)


#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)


add_library(${PROJECT_NAME} STATIC
    source/directory.cpp
    source/glob.cpp
    source/error.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)


if(ODAFS_TESTS)
    add_subdirectory(tests)
endif()
