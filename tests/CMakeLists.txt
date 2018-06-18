find_package(GTest REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)

# main() as library
add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main
    PUBLIC GTest::GTest
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)


# Test readDirectory
add_executable(test_odaFS_read_directory test_read_directory.cpp)
target_link_libraries(test_odaFS_read_directory
    odaFS
    test_main
)
set_target_properties(test_odaFS_read_directory
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_read_directory
    COMMAND test_odaFS_read_directory
    WORKING_DIRECTORY bin
)


# Test createDirectory
add_executable(test_odaFS_create_directory test_create_directory.cpp)
target_link_libraries(test_odaFS_create_directory
    odaFS
    test_main
)
set_target_properties(test_odaFS_create_directory
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_create_directory
    COMMAND test_odaFS_create_directory
    WORKING_DIRECTORY bin
)


# DataSet as library
add_library(DataSet STATIC dataset.cpp)
target_link_libraries(DataSet PRIVATE odaFS)
set_target_properties(DataSet PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Test DataSet
add_executable(test_odaFS_dataset test_dataset.cpp)
target_link_libraries(test_odaFS_dataset
    odaFS
    DataSet
    test_main
)
set_target_properties(test_odaFS_dataset
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_dataset
    COMMAND test_odaFS_dataset
    WORKING_DIRECTORY bin
)


# Test DataSetFiles
add_executable(test_odaFS_datasetfiles test_datasetfiles.cpp)
target_link_libraries(test_odaFS_datasetfiles
    odaFS
    DataSet
    test_main
)
set_target_properties(test_odaFS_datasetfiles
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_datasetfiles
    COMMAND test_odaFS_datasetfiles
    WORKING_DIRECTORY bin
)


# Test readFile
add_executable(test_odaFS_read_file test_read_file.cpp)
target_link_libraries(test_odaFS_read_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_odaFS_read_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_read_file
    COMMAND test_odaFS_read_file
    WORKING_DIRECTORY bin
)


# Test writeFile
add_executable(test_odaFS_write_file test_write_file.cpp)
target_link_libraries(test_odaFS_write_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_odaFS_write_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_write_file
    COMMAND test_odaFS_write_file
    WORKING_DIRECTORY bin
)


# Test read-write file
add_executable(test_odaFS_rw_file test_rw_file.cpp)
target_link_libraries(test_odaFS_rw_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_odaFS_rw_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_rw_file
    COMMAND test_odaFS_rw_file
    WORKING_DIRECTORY bin
)


# Test remove
add_executable(test_odaFS_remove test_remove.cpp)
target_link_libraries(test_odaFS_remove
    odaFS
    test_main
)
set_target_properties(test_odaFS_remove
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_remove
    COMMAND test_odaFS_remove
    WORKING_DIRECTORY bin
)


# Test remove
add_executable(test_odaFS_rename test_rename.cpp)
target_link_libraries(test_odaFS_rename
    odaFS
    test_main
)
set_target_properties(test_odaFS_rename
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_rename
    COMMAND test_odaFS_rename
    WORKING_DIRECTORY bin
)


# Tests for unique internal object

# Additional shared library
add_library(test_odaFS_dll SHARED dll.cpp)
target_link_libraries(test_odaFS_dll PRIVATE odaFS)
target_compile_definitions(test_odaFS_dll PRIVATE -DTEST_DLL_BUILD)
set_target_properties(test_odaFS_dll
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)

# Test load additional library with linker
add_executable(test_odaFS_dll_linker_load test_dll_linker_load.cpp)
target_link_libraries(test_odaFS_dll_linker_load
    odaFS
    test_odaFS_dll
    test_main
)
set_target_properties(test_odaFS_dll_linker_load
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_dll_linker_load
    COMMAND test_odaFS_dll_linker_load
    WORKING_DIRECTORY bin
)

# Test load additional library with manual
add_executable(test_odaFS_dll_manual_load test_dll_manual_load.cpp)
target_link_libraries(test_odaFS_dll_manual_load
    odaFS
    ${CMAKE_DL_LIBS}
    test_main
)
set_target_properties(test_odaFS_dll_manual_load
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_odaFS_dll_manual_load
    COMMAND test_odaFS_dll_manual_load
    WORKING_DIRECTORY bin
)
