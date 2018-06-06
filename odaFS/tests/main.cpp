#include <gtest/gtest.h>
#include <boost/locale/generator.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <locale>
#include <cstdlib>
#include <ctime>


struct separated : std::numpunct<char>
{
    virtual std::string do_grouping() const {
        return "\03";
    }
};


int main(int argc, char** argv) {

    std::locale defaultLocale{boost::locale::generator().generate("")};
    std::locale::global(defaultLocale);
    std::cout.imbue(defaultLocale);
    std::wcout.imbue(defaultLocale);
    boost::filesystem::path::imbue(defaultLocale);

    ::testing::InitGoogleTest(&argc, argv);

    std::locale separatedLocale{std::cout.getloc(), new separated};
    std::cout.imbue(separatedLocale);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    return RUN_ALL_TESTS();
}
