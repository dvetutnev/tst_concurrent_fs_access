#include <gtest/gtest.h>


#include <iostream>
#include <locale>


struct separated : std::numpunct<char>
{
    virtual std::string do_grouping() const {
        return "\03";
    }
};


int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);

    std::locale loc{std::cout.getloc(), new separated};
    std::cout.imbue(loc);

    return RUN_ALL_TESTS();
}
