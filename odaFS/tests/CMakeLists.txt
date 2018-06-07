project(odaFS_tests CXX)
cmake_minimum_required(VERSION 3.0)


find_package(GTest REQUIRED)

# main() as library
add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main
    PUBLIC GTest::GTest
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)


# Test readDirectory
add_executable(test_read_directory test_read_directory.cpp)
target_link_libraries(test_read_directory
    odaFS
    test_main
)
add_test(
    NAME test_read_directory
    COMMAND test_read_directory
    WORKING_DIRECTORY bin
)


# Test createDirectory
add_executable(test_create_directory test_create_directory.cpp)
target_link_libraries(test_create_directory
    odaFS
    test_main
)
add_test(
    NAME test_create_directory
    COMMAND test_create_directory
    WORKING_DIRECTORY bin
)


# DataSet as library
add_library(DataSet STATIC dataset.cpp)
target_link_libraries(DataSet PRIVATE odaFS)


# Test DataSet
add_executable(test_dataset test_dataset.cpp)
target_link_libraries(test_dataset
    odaFS
    DataSet
    test_main
)


# Test DataSetFiles
add_executable(test_datasetfiles test_datasetfiles.cpp)
target_link_libraries(test_datasetfiles
    odaFS
    DataSet
    test_main
)


# Test readFile
add_executable(test_read_file test_read_file.cpp)
target_link_libraries(test_read_file
    odaFS
    DataSet
    test_main
)
