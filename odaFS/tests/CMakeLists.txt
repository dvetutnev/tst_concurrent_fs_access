project(odaFS_tests CXX)
cmake_minimum_required(VERSION 3.0)


find_package(GTest REQUIRED)


# Test readDirectory
add_executable(test_read_directory
    test_read_directory.cpp
    test_main.cpp
)

target_link_libraries(test_read_directory
    odaFS
    GTest::GTest
)

add_test(
    NAME test_read_directory
    COMMAND test_read_directory
    WORKING_DIRECTORY bin
)


# Test createDirectory
add_executable(test_create_directory
    test_create_directory.cpp
    test_main.cpp
)

target_link_libraries(test_create_directory
    odaFS
    GTest::GTest
)

add_test(
    NAME test_create_directory
    COMMAND test_create_directory
    WORKING_DIRECTORY bin
)


# Test DataSet
add_executable(test_dataset
    dataset.cpp
    test_dataset.cpp
    test_main.cpp
)

target_link_libraries(test_dataset
    odaFS
    GTest::GTest
)
