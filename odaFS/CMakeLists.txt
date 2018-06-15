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

set(${PROJECT_NAME}_DLLMAIN)
if(WIN32)
    set(${PROJECT_NAME}_DLLMAIN source/dllmain_win.cpp)
endif()

add_library(${PROJECT_NAME} SHARED
    source/operations.cpp
    source/glob.cpp
    source/lock.cpp
    ${${PROJECT_NAME}_DLLMAIN}
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
