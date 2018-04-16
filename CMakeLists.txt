project(tst_cuncurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


include(conan.cmake)
conan_cmake_run(
    REQUIRES googletest/1.9.0-b2@odant/stable
    BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS UPDATE
    BUILD outdated
)


add_executable(main
    main.cpp
    generate_file.cpp
    fs_lock.cpp
)
