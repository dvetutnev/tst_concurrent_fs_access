/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"

#include <boost/locale/conversion.hpp>

#include <iostream>

namespace oda {
namespace fs {


Path currentDirectory() {

    return boost::filesystem::current_path();
}


namespace internal {


class Glob
{
public:

    Glob(const Path&, CaseSensitive);
    const Path& dirPath() const;
    bool match(const Path&) const;

private:

    template<typename It>
    bool match(It, It, It, It) const;

    const bool _caseSensitive;

    Path _patternPath;
    Path _dirPath;
};


} // namspace internal


Directory readDirectory(const Path& path, CaseSensitive caseSensitive) {

    Directory result;

    try {

        internal::Glob glob{path, caseSensitive};
        boost::filesystem::directory_iterator it{glob.dirPath()};
        const boost::filesystem::directory_iterator end;

        for (; it != end; ++it) {

            const auto& entryPath = it->path();

            if (glob.match(entryPath)) {

                const auto& fileStatus = it->status();
                const bool isDirectory = fileStatus.type() == boost::filesystem::directory_file;

                result.push_back(DirectoryEntry{entryPath, isDirectory});
            }
        }
    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path.string());
    }

    return result;
}


namespace internal {


Glob::Glob(const Path& path, CaseSensitive caseSensitive)
    : _caseSensitive{ (caseSensitive == CaseSensitive::True) ? true : false}
{
    const std::string& tail = path.filename().string();
    if (tail.find_first_of("*?") != std::string::npos) {

        _patternPath = (_caseSensitive) ? path : Path{boost::locale::to_lower(path.native())};
        _dirPath = path.parent_path();

    } else {

        _dirPath = path;
    }
}

const Path& Glob::dirPath() const {

    return _dirPath;
}

bool Glob::match(const Path& entryPath) const {

    if (_patternPath.empty()) {

        return true;
    }

    const auto& pattern = _patternPath.native();
    const auto& entry = (_caseSensitive) ? entryPath.native() : boost::locale::to_lower(entryPath.native());

    return match(std::cbegin(pattern), std::cend(pattern), std::cbegin(entry), std::cend(entry));
}

template<typename It>
bool Glob::match(It pIt, It pEndIt, It sIt, It sEndIt) const {

    while (pIt != pEndIt) {

        if (sIt == sEndIt) {

            while (pIt != pEndIt && *pIt == '*') {
                ++pIt;
            }
            break;
        }

        switch (*pIt) {

        case '?':

            ++pIt;
            ++sIt;
            break;

        case '*':

            if (++pIt != pEndIt) {

                while (sIt != sEndIt && !match(pIt, pEndIt, sIt, sEndIt)) {
                    ++sIt;
                }
                return sIt != sEndIt;
            }

            return true;

        default:

            if (*pIt != *sIt) {

                return false;
            }

            ++pIt;
            ++sIt;
        }
    }

    return pIt == pEndIt && sIt == sEndIt;
}

} // namespace internal


void createDirectory(const Path& path) {

    try {
        boost::filesystem::create_directory(path);

    } catch (const boost::filesystem::filesystem_error& ex) {

        internal::reThrow(ex, path.string());
    }
}


} // namespace fs
} // namespace oda