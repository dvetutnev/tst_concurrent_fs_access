project(tst_concurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)


include(conan.cmake)
conan_cmake_run(
    REQUIRES
        googletest/1.9.0-b2@odant/stable
        easy_profiler/2.0.0@odant/stable
        boost/1.67.0@odant/testing
    BASIC_SETUP
    NO_OUTPUT_DIRS
    UPDATE
    BUILD
        outdated
)


find_package(GTest REQUIRED)
find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    filesystem
    locale
)

if(MSVC)
    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
    )
endif()

enable_testing()

#add_executable(main
#    app.cpp
#    generate_file.cpp
#    fs_lock.cpp
#    main.cpp
#)
#target_link_libraries(main GTest::GTest)

include_directories(include)

# Worker
add_executable(test_worker
    test_worker.cpp
    worker.cpp
    generate.cpp
    main.cpp
)

target_link_libraries(test_worker
    GTest::GTest
    Boost::locale
    Boost::system
    easy_profiler
)

add_test(
    NAME test_worker
    COMMAND test_worker
    WORKING_DIRECTORY bin
)


# DataSet
add_executable(test_data_set
    test_data_set.cpp
    data_set.cpp
    generate.cpp
    main.cpp
)

target_compile_definitions(test_data_set
    PUBLIC -DDUMMY
    #PUBLIC -DDISABLE_EASY_PROFILER
)

target_link_libraries(test_data_set
    GTest::GTest
    Boost::locale
    Boost::system
    easy_profiler
)

add_test(
    NAME test_data_set
    COMMAND test_data_set
    WORKING_DIRECTORY bin
)


# FS errors
add_executable(test_fs_errors
    test_fs_errors.cpp
    main.cpp
)

target_link_libraries(test_fs_errors
    GTest::GTest
    Boost::locale
    Boost::filesystem
)

add_test(
    NAME test_fs_errors
    COMMAND test_fs_errors
    WORKING_DIRECTORY bin
)


# Test utf path
configure_file(Кириллица.txt bin/Кириллица.txt COPYONLY)

add_executable(test_path_utf
    test_path_utf.cpp
    main.cpp
)

target_link_libraries(test_path_utf
    GTest::GTest
    Boost::locale
    Boost::filesystem
)

add_test(
    NAME test_path_utf
    COMMAND test_path_utf
    WORKING_DIRECTORY bin
)


# Test readDirectory
add_executable(test_read_directory
    fs/directory.cpp
    #
    test_read_directory.cpp
    main.cpp
)

target_link_libraries(test_read_directory
    GTest::GTest
    Boost::locale
    Boost::filesystem
)

add_test(
    NAME test_read_directory
    COMMAND test_read_directory
    WORKING_DIRECTORY bin
)
