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

    {   // Setup default locale
        const boost::locale::generator gen;
        const std::locale loc = gen.generate(std::locale(), "");
        std::locale::global(loc);
        std::cout.imbue(loc);
        std::wcout.imbue(loc);
        boost::filesystem::path::imbue(loc);
    }

    ::testing::InitGoogleTest(&argc, argv);

    std::locale separatedLocale{std::cout.getloc(), new separated};
    std::cout.imbue(separatedLocale);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    return RUN_ALL_TESTS();
}
