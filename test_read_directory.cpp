#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <fs/directory.h>

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
        const boost::filesystem::path path = _basePath / "dir1";
        boost::filesystem::create_directory(path);
    }
    {
        const boost::filesystem::path path = _basePath / "dir2";
        boost::filesystem::create_directory(path);
    }
}

TestDirectory::~TestDirectory() {

    boost::filesystem::remove_all(_basePath);
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

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path path = currentPath / "test_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
        { path / "b3.bin",  false },
        { path / "dir1",    true },
        { path / "dir2",    true }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, error_not_exists) {

    const boost::filesystem::path path = boost::filesystem::current_path() / "not_exists";
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
    const boost::filesystem::path path = "C:\\Users\\Default\\Application Data";
#elif
    const boost::filesystem::path path = "/root";
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

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path path = currentPath / "not_direcory";
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


TEST(readDirectory, wildcard) {

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path path = currentPath / "test_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
        { path / "b1.txt",  false },
        { path / "b2.txt",  false },
        { path / "b3.bin",  false },
        { path / "dir1",    true },
        { path / "dir2",    true }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "*");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}


TEST(readDirectory, wildcard_tail) {

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path path = currentPath / "test_read_directory";
    TestDirectory testDirectory{path};

    oda::fs::Directory normalResult{
        { path / "a1.txt",  false },
        { path / "a2.txt",  false },
        { path / "a3.bin",  false },
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(path / "a*");
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}
