#include <gtest/gtest.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/locale.hpp>

#include <string>
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

    const std::string filepath_utf8 = "Кириллица.txt";
    const std::wstring filepath_utf16 = boost::locale::conv::utf_to_utf<wchar_t>(filepath_utf8);
    boost::filesystem::path filepath = filepath_utf16;
    boost::filesystem::ifstream f{filepath};
    ASSERT_TRUE(f.is_open());
}
