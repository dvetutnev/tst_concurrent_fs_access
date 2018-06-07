project(odaFS CXX)
cmake_minimum_required(VERSION 3.0)


option(ODA_FS_TESTS "Enable odaFS tests" ON)


#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)


add_library(${PROJECT_NAME} STATIC
    source/file.cpp
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


if(ODA_FS_TESTS)
    add_subdirectory(tests)
endif()
