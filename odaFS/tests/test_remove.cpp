#include <fs/fs.h>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>


TEST(remove, file_simple) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_remove_file";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    boost::filesystem::ofstream file{path};
    file << "test_remove_file";
    file.close();

    oda::fs::remove(path);
    ASSERT_FALSE(boost::filesystem::exists(path));
}


TEST(remove, directory_simple) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_remove_directory";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    boost::filesystem::create_directory(path);

    oda::fs::remove(path);
    ASSERT_FALSE(boost::filesystem::exists(path));
}


TEST(remove, not_empty) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_remove_not_empty";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    boost::filesystem::create_directory(path);
    boost::filesystem::ofstream file{path / "not_empty"};
    file << "test_remove_not_empty";
    file.close();

    try {

        oda::fs::remove(path);
        FAIL();

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }
    SUCCEED();
}


TEST(remove, access_denied) {

#ifdef _WIN32
    const oda::fs::Path path = "C:\\System Volume Information";
#else
    const oda::fs::Path path = "/root";
#endif

    try {

        oda::fs::remove(path);
        FAIL() << "No throw exception!";

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }

    SUCCEED();
}


TEST(removeAll, not_empty) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "test_remove_not_empty";

    if (boost::filesystem::exists(path)) {

        boost::filesystem::remove_all(path);
    }

    boost::filesystem::create_directory(path);
    boost::filesystem::ofstream file{path / "not_empty"};
    file << "test_remove_not_empty";
    file.close();

    oda::fs::removeAll(path);
    ASSERT_FALSE(boost::filesystem::exists(path));
}


TEST(removeAll, access_denied) {

#ifdef _WIN32
    const oda::fs::Path path = "C:\\System Volume Information";
#else
    const oda::fs::Path path = "/root";
#endif

    try {

        oda::fs::removeAll(path);
        FAIL() << "No throw exception!";

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }

    SUCCEED();
}
