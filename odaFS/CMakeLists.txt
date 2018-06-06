project(odaFS CXX)
cmake_minimum_required(VERSION 3.0)


option(ODAFS_TESTS "Enable odaFS unittests" ON)


#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
)


add_library(${PROJECT_NAME} STATIC
    source/directory.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
)


if(ODAFS_TESTS)
    add_subdirectory(tests)
endif()
