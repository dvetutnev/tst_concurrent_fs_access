project(tst_concurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


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


if(MSVC)
    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
    )
endif()


#add_executable(main
#    app.cpp
#    generate_file.cpp
#    fs_lock.cpp
#    main.cpp
#)
#target_link_libraries(main GTest::GTest)


add_executable(test_worker
    test_worker.cpp
    worker.cpp
    generate.cpp
    main.cpp
)

target_link_libraries(test_worker
    GTest::GTest
    easy_profiler
)


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
    easy_profiler
)


find_package(Boost REQUIRED filesystem)

add_executable(test_fs_errors
        test_fs_errors.cpp
        main.cpp
)

target_link_libraries(test_fs_errors
    GTest::GTest
    Boost::filesystem
)
