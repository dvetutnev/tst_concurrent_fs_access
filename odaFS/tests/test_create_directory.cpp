#include <fs/directory.h>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>


TEST(createDirectory, normal) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_create_directory";

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
    const oda::fs::Path path = currentDirectory / "test_create_directory_already_exists_direcory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    boost::filesystem::create_directory(path);

    oda::fs::createDirectory(path);

    if (!boost::filesystem::is_directory(path)) {

        FAIL() << "Directory not created!";
    }

    boost::filesystem::remove_all(path);
    SUCCEED();
}


TEST(createDirectory, already_exists_file) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_create_directory_already_exists_file";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }
    {
        boost::filesystem::ofstream f{path};
        f << "already_exists_file";
    }

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
