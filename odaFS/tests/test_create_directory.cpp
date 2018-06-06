#include <fs/directory.h>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>


TEST(createDirectory, normal) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_createDirectory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }


    oda::fs::createDirectory(path);

    if (!boost::filesystem::is_directory(path)) {

        FAIL() << "Directory not created!";
    }

    boost::filesystem::remove_all(path);
    SUCCEED();
}


TEST(createDirectory, already_exists_directory) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "already_exists_direcory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    boost::filesystem::create_directory(path);

    try {

        oda::fs::createDirectory(path);
        FAIL();

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }
}
