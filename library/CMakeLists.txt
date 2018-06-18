#find_package(easy_profiler REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        filesystem
        locale
        thread
)

set(odaFS_DLLMAIN)
if(WIN32)
    set(odaFS_DLLMAIN source/dllmain_win.cpp)
endif()

add_library(odaFS SHARED
    source/operations.cpp
    source/glob.cpp
    source/lock.cpp
    ${${PROJECT_NAME}_DLLMAIN}
)

target_include_directories(odaFS PUBLIC include)
target_compile_definitions(odaFS PRIVATE -DODA_FS_BUILD)
set_target_properties(odaFS
    PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
)
target_link_libraries(odaFS
    PUBLIC Boost::filesystem
    PUBLIC Boost::locale
    PUBLIC Boost::thread
)

if(ODA_FS_FAKE_LOCK)
    target_compile_definitions(odaFS PUBLIC -DODA_FS_FAKE_LOCK)
endif()

if(ODA_FS_TESTS)
    target_compile_definitions(odaFS PUBLIC -DODA_FS_TESTS)
endif()

