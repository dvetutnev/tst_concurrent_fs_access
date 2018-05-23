#include <gtest/gtest.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <system_error>
#include <cerrno>
#include <locale>

#include <common/exception.h>


namespace oda {
namespace fs {


class Exception : public oda::error::Exception
{
    using oda::error::Exception::Exception;
};


}   // namespace fs
}   // namespace oda


TEST(oda_exception, error_code) {

    try {

        const std::error_code ec = std::make_error_code(std::errc::directory_not_empty);
        throw oda::fs::Exception{ec};

        FAIL();

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;
        std::cout << "ec.message(): " << ec.message() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
        std::cout << "ecat.message( ec.value() ): " << ecat.message( ec.value() ) << std::endl;
    };

    SUCCEED();
}


TEST(oda_exception, error_code_what_arg) {

    try {

        const std::error_code ec = std::make_error_code(std::errc::permission_denied);
        throw oda::fs::Exception{ec, "/root/.bashrc"};

        FAIL();

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;
        std::cout << "ec.message(): " << ec.message() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
        std::cout << "ecat.message( ec.value() ): " << ecat.message( ec.value() ) << std::endl;
    };

    SUCCEED();
}


TEST(FSErrors, boost_open_file) {

    boost::filesystem::path filepath = "not_exists";

    boost::filesystem::ifstream f{filepath};
    ASSERT_FALSE(f.is_open());

    std::cout << "errno: " << errno << std::endl;
    std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
    ASSERT_TRUE(ec);

    std::cout << "ec: " << ec << std::endl;
    std::cout << "ec.value(): " << ec.value() << std::endl;
    std::cout << "ec.category().name: " << ec.category().name() << std::endl;
    std::cout << "ec.message(): " << ec.message() << std::endl;

    try {

        throw oda::fs::Exception{ec};

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    try {

        throw oda::fs::Exception{ec, filepath.generic_string()};

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    SUCCEED();
}

TEST(FSErrors, std_open_file) {

    const std::string filepath = "not_exists";

    std::ifstream f{filepath};
    ASSERT_FALSE(f.is_open());

    std::cout << "errno: " << errno << std::endl;

    std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
    ASSERT_TRUE(ec);
    std::cout << "ec: " << ec << std::endl;
    std::cout << "ec.value(): " << ec.value() << std::endl;
    std::cout << "ec.category().name: " << ec.category().name() << std::endl;
    std::cout << "ec.message(): " << ec.message() << std::endl;

    try {

        throw oda::fs::Exception{ec};

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    try {

        throw oda::fs::Exception{ec, filepath};

    } catch (const oda::error::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    SUCCEED();
}
