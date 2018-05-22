#include <gtest/gtest.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/locale/generator.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <system_error>
#include <cerrno>
#include <locale>


TEST(FS, cyrillic_filepath) {

    std::locale loc{boost::locale::generator().generate("")};
    boost::filesystem::path::imbue(loc);

    boost::filesystem::path filepath = "Кириллица.txt";
    boost::filesystem::ifstream f{filepath};
    ASSERT_TRUE(f.is_open());
}


TEST(FS, cyrillic_filepath_wchar) {

    std::locale loc{boost::locale::generator().generate("")};
    boost::filesystem::path::imbue(loc);

    boost::filesystem::path filepath = L"Кириллица.txt";
    boost::filesystem::ifstream f{filepath};
    ASSERT_TRUE(f.is_open());
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

        throw std::system_error{ec};
    } catch (const std::system_error& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    try {

        throw std::system_error{ec, filepath.generic_string()};
    } catch (const std::system_error& e) {

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

        throw std::system_error{ec};
    } catch (const std::system_error& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    try {

        throw std::system_error{ec, filepath};
    } catch (const std::system_error& e) {

        std::cout << "e.what(): " << e.what() << std::endl;
    };

    SUCCEED();
}
