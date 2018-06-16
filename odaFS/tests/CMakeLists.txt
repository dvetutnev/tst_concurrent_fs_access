find_package(GTest REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test_${PROJECT_NAME}_dir)

# main() as library
add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main
    PUBLIC GTest::GTest
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)


# Test readDirectory
add_executable(test_${PROJECT_NAME}_read_directory test_read_directory.cpp)
target_link_libraries(test_${PROJECT_NAME}_read_directory
    odaFS
    test_main
)
set_target_properties(test_${PROJECT_NAME}_read_directory
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_read_directory
    COMMAND test_${PROJECT_NAME}_read_directory
    WORKING_DIRECTORY bin
)


# Test createDirectory
add_executable(test_${PROJECT_NAME}_create_directory test_create_directory.cpp)
target_link_libraries(test_${PROJECT_NAME}_create_directory
    odaFS
    test_main
)
set_target_properties(test_${PROJECT_NAME}_create_directory
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_create_directory
    COMMAND test_${PROJECT_NAME}_create_directory
    WORKING_DIRECTORY bin
)


# DataSet as library
add_library(DataSet STATIC dataset.cpp)
target_link_libraries(DataSet PRIVATE odaFS)
set_target_properties(DataSet PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Test DataSet
add_executable(test_${PROJECT_NAME}_dataset test_dataset.cpp)
target_link_libraries(test_${PROJECT_NAME}_dataset
    odaFS
    DataSet
    test_main
)
set_target_properties(test_${PROJECT_NAME}_dataset
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_dataset
    COMMAND test_${PROJECT_NAME}_dataset
    WORKING_DIRECTORY bin
)


# Test DataSetFiles
add_executable(test_${PROJECT_NAME}_datasetfiles test_datasetfiles.cpp)
target_link_libraries(test_${PROJECT_NAME}_datasetfiles
    odaFS
    DataSet
    test_main
)
set_target_properties(test_${PROJECT_NAME}_datasetfiles
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_datasetfiles
    COMMAND test_${PROJECT_NAME}_datasetfiles
    WORKING_DIRECTORY bin
)


# Test readFile
add_executable(test_${PROJECT_NAME}_read_file test_read_file.cpp)
target_link_libraries(test_${PROJECT_NAME}_read_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_${PROJECT_NAME}_read_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_read_file
    COMMAND test_${PROJECT_NAME}_read_file
    WORKING_DIRECTORY bin
)


# Test writeFile
add_executable(test_${PROJECT_NAME}_write_file test_write_file.cpp)
target_link_libraries(test_${PROJECT_NAME}_write_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_${PROJECT_NAME}_write_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_write_file
    COMMAND test_${PROJECT_NAME}_write_file
    WORKING_DIRECTORY bin
)


# Test read-write file
add_executable(test_${PROJECT_NAME}_rw_file test_rw_file.cpp)
target_link_libraries(test_${PROJECT_NAME}_rw_file
    odaFS
    DataSet
    test_main
)
set_target_properties(test_${PROJECT_NAME}_rw_file
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_rw_file
    COMMAND test_${PROJECT_NAME}_rw_file
    WORKING_DIRECTORY bin
)


# Test remove
add_executable(test_${PROJECT_NAME}_remove test_remove.cpp)
target_link_libraries(test_${PROJECT_NAME}_remove
    odaFS
    test_main
)
set_target_properties(test_${PROJECT_NAME}_remove
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_remove
    COMMAND test_${PROJECT_NAME}_remove
    WORKING_DIRECTORY bin
)


# Test remove
add_executable(test_${PROJECT_NAME}_rename test_rename.cpp)
target_link_libraries(test_${PROJECT_NAME}_rename
    odaFS
    test_main
)
set_target_properties(test_${PROJECT_NAME}_rename
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_rename
    COMMAND test_${PROJECT_NAME}_rename
    WORKING_DIRECTORY bin
)


# Tests for unique internal object

# Additional shared library
add_library(test_${PROJECT_NAME}_dll SHARED dll.cpp)
target_link_libraries(test_${PROJECT_NAME}_dll PRIVATE odaFS)
target_compile_definitions(test_${PROJECT_NAME}_dll PRIVATE -DTEST_DLL_BUILD)
set_target_properties(test_${PROJECT_NAME}_dll
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)

# Test load additional library with linker
add_executable(test_${PROJECT_NAME}_dll_linker_load test_dll_linker_load.cpp)
target_link_libraries(test_${PROJECT_NAME}_dll_linker_load
    odaFS
    test_${PROJECT_NAME}_dll
    test_main
)
set_target_properties(test_${PROJECT_NAME}_dll_linker_load
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_dll_linker_load
    COMMAND test_${PROJECT_NAME}_dll_linker_load
    WORKING_DIRECTORY bin
)

# Test load additional library with manual
add_executable(test_${PROJECT_NAME}_dll_manual_load test_dll_manual_load.cpp)
target_link_libraries(test_${PROJECT_NAME}_dll_manual_load
    ${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    test_main
)
set_target_properties(test_${PROJECT_NAME}_dll_manual_load
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_${PROJECT_NAME}_dll_manual_load
    COMMAND test_${PROJECT_NAME}_dll_manual_load
    WORKING_DIRECTORY bin
)
