project(odaFS_tests CXX)
cmake_minimum_required(VERSION 3.0)


find_package(GTest REQUIRED)


add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main
    PUBLIC GTest::GTest
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
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


# Test DataSet
add_executable(test_dataset dataset.cpp test_dataset.cpp)
target_link_libraries(test_dataset
    odaFS
    test_main
)


# Test DataSetFiles
add_executable(test_datasetfiles dataset.cpp  test_datasetfiles.cpp)
target_link_libraries(test_datasetfiles
    odaFS
    test_main
)


# Test IO files
add_executable(test_io_files test_io_files.cpp)
target_link_libraries(test_io_files
    odaFS
    test_main
)
