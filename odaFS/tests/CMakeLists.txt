project(odaFS_tests CXX)
cmake_minimum_required(VERSION 3.0)


find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)


# Test readDirectory
add_executable(test_read_directory
    test_read_directory.cpp
    main.cpp
)

target_link_libraries(test_read_directory
    odaFS
    GTest::GTest
    Boost::thread
)

add_test(
    NAME test_read_directory
    COMMAND test_read_directory
    WORKING_DIRECTORY bin
)
