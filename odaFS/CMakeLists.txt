#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)

set(${PROJECT_NAME}_DLLMAIN)
if(WIN32)
    set(${PROJECT_NAME}_DLLMAIN source/dllmain_win.cpp)
endif()

add_library(${PROJECT_NAME} SHARED
    source/operations.cpp
    source/glob.cpp
    source/lock.cpp
    ${${PROJECT_NAME}_DLLMAIN}
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DODA_FS_BUILD)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)

if(ODA_FS_FAKE_LOCK)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DODA_FS_FAKE_LOCK)
endif()

if(ODA_FS_TESTS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DODA_FS_TESTS)
    add_subdirectory(tests)
endif()
