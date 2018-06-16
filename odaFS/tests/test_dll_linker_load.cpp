/*
 * ODA filesystem. Test for unique internal object.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "dll.h"
#include <fs/fs.h>

#include <gtest/gtest.h>

#include <iostream>


TEST(testDLL_linker_load, fake) {

    SUCCEED();
}


#ifdef ODA_FS_TESTS

TEST(testDLL_linker_load, address_and_size__insert_from_EXE) {

    void* const addressFromEXE = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL = DLLaddressOfLocks();

    const std::size_t sizeFromEXE = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL = DLLsizeOfLocks();

    ASSERT_EQ(addressFromEXE, addressFromDLL);
    ASSERT_EQ(sizeFromEXE, sizeFromDLL);


    oda::fs::internal::insertDummyLock();


    void* const addressFromEXE_after_insert = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL_after_insert = DLLaddressOfLocks();

    const std::size_t sizeFromEXE_after_insert = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL_after_insert = DLLsizeOfLocks();

    ASSERT_EQ(addressFromEXE_after_insert, addressFromDLL_after_insert);
    ASSERT_EQ(sizeFromEXE_after_insert, sizeFromDLL_after_insert);
}


TEST(testDLL_linker_load, address_and_size__insert_from_DLL) {

    void* const addressFromEXE = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL = DLLaddressOfLocks();

    const std::size_t sizeFromEXE = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL = DLLsizeOfLocks();

    ASSERT_EQ(addressFromEXE, addressFromDLL);
    ASSERT_EQ(sizeFromEXE, sizeFromDLL);


    DLLinsertDummyLock();


    void* const addressFromEXE_after_insert = oda::fs::internal::addressOfLocks();
    void* const addressFromDLL_after_insert = DLLaddressOfLocks();

    const std::size_t sizeFromEXE_after_insert = oda::fs::internal::sizeOfLocks();
    const std::size_t sizeFromDLL_after_insert = DLLsizeOfLocks();

    ASSERT_EQ(addressFromEXE_after_insert, addressFromDLL_after_insert);
    ASSERT_EQ(sizeFromEXE_after_insert, sizeFromDLL_after_insert);
}


TEST(testDLL_linker_load, print) {

    std::cout << "From EXE address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLLprint();

    std::cout << "Insert dummy lock..." << std::endl;
    oda::fs::internal::insertDummyLock();

    std::cout << "From EXE address: " << oda::fs::internal::addressOfLocks() << " size: " << oda::fs::internal::sizeOfLocks() << std::endl;
    DLLprint();

    SUCCEED();
}

#endif // ODA_FS_TESTS
