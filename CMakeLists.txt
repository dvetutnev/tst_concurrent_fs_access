project(tst_cuncurrent_fs_access CXX)
cmake_minimum_required(VERSION 3.0)


add_executable(main
    main.cpp
    generate_file.cpp
    fs_lock.cpp
)
