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


TEST(Fake, fake) {

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path basePath = currentPath / "test_read_directory";
    TestDirectory testDirectory{basePath};
}


TEST(readDirectory, simple) {

    const boost::filesystem::path currentPath = boost::filesystem::current_path();
    const boost::filesystem::path basePath = currentPath / "test_read_directory";
    TestDirectory testDirectory{basePath};

    oda::fs::Directory normalResult{
        { basePath / "a1.txt",  false },
        { basePath / "a2.txt",  false },
        { basePath / "a3.bin",  false },
        { basePath / "b1.txt",  false },
        { basePath / "b2.txt",  false },
        { basePath / "b3.bin",  false },
        { basePath / "dir1",    true },
        { basePath / "dir2",    true }
    };
    std::sort(std::begin(normalResult), std::end(normalResult));

    oda::fs::Directory result = oda::fs::readDirectory(basePath);
    std::sort(std::begin(result), std::end(result));

    ASSERT_EQ(result, normalResult);
}
