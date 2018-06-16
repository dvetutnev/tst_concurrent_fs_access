#include <fs/fs.h>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>


TEST(rename, not_exists) {

    const oda::fs::Path prefixPath = oda::fs::currentDirectory() / "test_odaFS_dir";
    boost::filesystem::create_directories(prefixPath);
    const oda::fs::Path oldPath = prefixPath / "test_rename_not_exists";

    if (boost::filesystem::exists(oldPath)) {

        boost::filesystem::remove_all(oldPath);
    }

    const oda::fs::Path newPath = prefixPath / "test_rename_new_path";

    if (boost::filesystem::exists(newPath)) {

        boost::filesystem::remove_all(newPath);
    }

    try {

        oda::fs::rename(oldPath, newPath);
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
