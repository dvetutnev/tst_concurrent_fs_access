project(tst_concurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


include(conan.cmake)
conan_cmake_run(
    REQUIRES
        googletest/1.9.0-b2@odant/stable
        easy_profiler/2.0.0@odant/stable
        boost/1.67.0@odant/testing
    BASIC_SETUP
    CMAKE_TARGETS
    NO_OUTPUT_DIRS
    UPDATE
    BUILD
        outdated
    IMPORTS
        "bin, *.dll -> ./bin"
        "bin, *.pdb -> ./bin"
        "lib, *.so* -> ./bin"
)



if(MSVC)
    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
    )
endif()


include_directories(include)


enable_testing()


add_subdirectory(odaFS)

#add_executable(main
#    app.cpp
#    generate_file.cpp
#    fs_lock.cpp
#    main.cpp
#)
#target_link_libraries(main GTest::GTest)

# Worker
#add_executable(test_worker
#    test_worker.cpp
#    worker.cpp
#    generate.cpp
#    main.cpp
#)

#target_link_libraries(test_worker
#    GTest::GTest
#    Boost::locale
#    Boost::filesystem
#    Boost::system
#    Boost::thread
#    easy_profiler
#)

#add_test(
#    NAME test_worker
#    COMMAND test_worker
#    WORKING_DIRECTORY bin
#)


# FS errors
#add_executable(test_fs_errors
#    test_fs_errors.cpp
#    main.cpp
#)

#target_link_libraries(test_fs_errors
#    GTest::GTest
#    Boost::locale
#    Boost::filesystem
#    Boost::thread
#)

#add_test(
#    NAME test_fs_errors
#    COMMAND test_fs_errors
#    WORKING_DIRECTORY bin
#)


# Test utf path
#configure_file(Кириллица.txt bin/Кириллица.txt COPYONLY)

#add_executable(test_path_utf
#    test_path_utf.cpp
#    main.cpp
#)

#target_link_libraries(test_path_utf
#    GTest::GTest
#    Boost::locale
#    Boost::filesystem
#    Boost::thread
#)

#add_test(
#    NAME test_path_utf
#    COMMAND test_path_utf
#    WORKING_DIRECTORY bin
#)

