project(odaFS_tests CXX)
cmake_minimum_required(VERSION 3.0)


find_package(GTest REQUIRED)


# Test readDirectory
add_executable(test_readDirectory
    test_readDirectory.cpp
    test_main.cpp
)

target_link_libraries(test_readDirectory
    odaFS
    GTest::GTest
)

add_test(
    NAME test_readDirectory
    COMMAND test_readDirectory
    WORKING_DIRECTORY bin
)


# Test createDirectory
add_executable(test_createDirectory
    test_createDirectory.cpp
    test_main.cpp
)

target_link_libraries(test_createDirectory
    odaFS
    GTest::GTest
)

add_test(
    NAME test_createDirectory
    COMMAND test_createDirectory
    WORKING_DIRECTORY bin
)
