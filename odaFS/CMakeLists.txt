project(odaFS CXX)
cmake_minimum_required(VERSION 3.0)


option(ODA_FS_TESTS "Enable odaFS tests" ON)
option(ODA_FS_FAKE_LOCK "Without thread-safe" OFF)


#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)


add_library(${PROJECT_NAME} SHARED
    source/operations.cpp
    source/glob.cpp
    source/lock.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DODA_FS_BUILD)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

if(ODA_FS_FAKE_LOCK)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DODA_FS_FAKE_LOCK)
endif()

if(ODA_FS_TESTS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DODA_FS_TESTS)
    add_subdirectory(tests)
endif()
