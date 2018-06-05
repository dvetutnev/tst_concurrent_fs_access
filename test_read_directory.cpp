#include <fs/directory.h>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <algorithm>


namespace {

class TestDirectory
{
public:

    TestDirectory(const boost::filesystem::path&);
    ~TestDirectory();

private:

    const boost::filesystem::path _basePath;
};

TestDirectory::TestDirectory(const boost::filesystem::path& p)
    : _basePath{p}
{
    if (boost::filesystem::exists(_basePath)) {

        boost::filesystem::remove_all(_basePath);
    }
    boost::filesystem::create_directory(_basePath);

    {
        const boost::filesystem::path path = _basePath / "a1.txt";
        boost::filesystem::ofstream file{path};
        file << "a1.txt";
    }
    {
        const boost::filesystem::path path = _basePath / "a2.txt";
        boost::filesystem::ofstream file{path};
        file << "a2.txt";
    }
    {
        const boost::filesystem::path path = _basePath / "a3.bin";
        boost::filesystem::ofstream file{path};
        file << "a3.bin";
    }
    {
        const boost::filesystem::path path = _basePath / "a33.bin";
        boost::filesystem::ofstream file{path};
        file << "a33.bin";
    }
    {
        const boost::filesystem::path path = _basePath / "b1.txt";
        boost::filesystem::ofstream file{path};
        file << "b1.txt";
    }
    {
        const boost::filesystem::path path = _basePath / "b2.txt";
        boost::filesystem::ofstream file{path};
        file << "b2.txt";
    }
    {
        const boost::filesystem::path path = _basePath / "b3.bin";
        boost::filesystem::ofstream file{path};
        file << "b3.bin";
    }
    {
        const boost::filesystem::path path = _basePath / "b33.bin";
        boost::filesystem::ofstream file{path};
        file << "b33.bin";
    }
    {
        const boost::filesystem::path path = _basePath / "dir1";
        boost::filesystem::create_directory(path);
    }
    {
        const boost::filesystem::path path = _basePath / "dir2";
        boost::filesystem::create_directory(path);
    }
}

TestDirectory::~TestDirectory() {

    //boost::filesystem::remove_all(_basePath);
}

} // anonymous namespace


namespace oda {
namespace fs {

// Test output
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<< (std::basic_ostream<CharT, Traits>& os, const DirectoryEntry& de) {

    os << ((de.isDirectory) ? "[d] " : "[f] ") << de.path;
    return os;
}


} // namespace fs
} // namespace oda


TEST(readDirectory, simple) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
        { path / "a33.bin", false },
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
        { path / "b3.bin",  false },
        { path / "b33.bin", false },
        { path / "dir1",    true },
        { path / "dir2",    true }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_asterisk) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
        { path / "a33.bin", false },
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
        { path / "b3.bin",  false },
        { path / "b33.bin", false },
        { path / "dir1",    true },
        { path / "dir2",    true }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "*");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_asterisk_tail) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
        { path / "a33.bin", false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "a*");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_asterisk_head) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a3.bin",  false },
        { path / "a33.bin", false },
        { path / "b3.bin",  false },
        { path / "b33.bin", false },
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "*.bin");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_asterisk_middle) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "b*.txt");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_question_mark) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "a?.txt");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_question_mark_and_asterisk) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
        { path / "b3.bin",  false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "b?.*");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_case_sensitive_true) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};
    {
        boost::filesystem::ofstream f1{path / "case_sensitive_1.tmp"};
        f1 << "case_sensitive_1.tmp";
        boost::filesystem::ofstream f2{path / "Case_Sensitive_2.tmp"};
        f2 << "Case_Sensitive_2.tmp";
    }

    oda::fs::Directory normalResult{
        { path / "Case_Sensitive_2.tmp",    false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "Case_Sensitive_*", oda::fs::CaseSensitive::True);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_case_sensitive_true_cyrillic) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};
    {
        boost::filesystem::ofstream f1{path / "файл_1.tmp"};
        f1 << "файл_1.tmp";
        boost::filesystem::ofstream f2{path / "фАйЛ_2.tmp"};
        f2 << "фАйЛ_2.tmp";
    }

    oda::fs::Directory normalResult{
        { path / "фАйЛ_2.tmp",    false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "фАйЛ_*", oda::fs::CaseSensitive::True);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_case_sensitive_false) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    TestDirectory testDirectory{path};
    {
        boost::filesystem::ofstream f1{path / "case_sensitive_1.tmp"};
        f1 << "case_sensitive_1.tmp";
        boost::filesystem::ofstream f2{path / "Case_Sensitive_2.tmp"};
        f2 << "Case_Sensitive_2.tmp";
    }

    oda::fs::Directory normalResult{
        { path / "case_sensitive_1.tmp",    false },
        { path / "Case_Sensitive_2.tmp",    false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "Case_Sensitive_*", oda::fs::CaseSensitive::False);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_case_sensitive_false_cyrillic) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "dataset_read_directory";
    //TestDirectory testDirectory{path};
    {
        boost::filesystem::ofstream f1{path / "файл_1.tmp"};
        f1 << "файл_1.tmp";
        boost::filesystem::ofstream f2{path / "фАйЛ_2.tmp"};
        f2 << "фАйЛ_2.tmp";
    }

    oda::fs::Directory normalResult{
        { path / "файл_1.tmp",    false },
        { path / "фАйЛ_2.tmp",    false }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "фАйЛ_*", oda::fs::CaseSensitive::False);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, error_not_exists) {

    const oda::fs::Path path = oda::fs::currentDirectory() / "not_exists";
    try {

        oda::fs::readDirectory(path);
        FAIL();

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }
}


TEST(readDirectory, error_access_denied) {

#ifdef _WIN32
    const oda::fs::Path path = "C:\\Users\\Default\\Application Data";
#else
    const oda::fs::Path path = "/root";
#endif
    try {

        oda::fs::readDirectory(path);
        FAIL();

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }
}


TEST(readDirectory, error_not_directory) {

    const oda::fs::Path currentDirectory = oda::fs::currentDirectory();
    const oda::fs::Path path = currentDirectory / "not_direcory";
    boost::filesystem::ofstream f{path};
    f << "not_directory";
    f.close();

    try {

        oda::fs::readDirectory(path);
        FAIL();

    } catch (const oda::fs::Exception& e) {

        std::cout << "e.what(): " << e.what() << std::endl;

        const std::error_code& ec = e.code();
        std::cout << "ec.value(): " << ec.value() << std::endl;

        const std::error_category& ecat = ec.category();
        std::cout << "ecat.name(): " << ecat.name() << std::endl;
    }
}
