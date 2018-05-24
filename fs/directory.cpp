/*
 * ODA filesystem. Directory operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/directory.h"

#include <boost/filesystem/convenience.hpp>

#include <cctype>
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

    bool match(std::string::const_iterator, std::string::const_iterator, std::string::const_iterator, std::string::const_iterator) const;

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
    } catch (const boost::filesystem::filesystem_error& e) {

        const boost::system::error_code& boostEc = e.code();
        const std::error_code stdEc = std::make_error_code(static_cast<std::errc>(boostEc.value()));
        throw Exception{stdEc, path.string()};
    }

    return result;
}


namespace internal {


Glob::Glob(const Path& path, CaseSensitive caseSensitive)
    : _caseSensitive{ (caseSensitive == CaseSensitive::True) ? true : false}
{

    const std::string& tail = path.filename().string();
    if (tail.find_first_of("*?") != std::string::npos) {

        _patternPath = path;
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

    const std::string& pattern = _patternPath.string();
    const std::string& entry = entryPath.string();

    return match(std::cbegin(pattern), std::cend(pattern), std::cbegin(entry), std::cend(entry));
}

bool Glob::match(std::string::const_iterator pIt, std::string::const_iterator pEndIt, std::string::const_iterator sIt, std::string::const_iterator sEndIt) const {

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

            if (_caseSensitive) {

                if (*pIt != *sIt) {

                    return false;
                }
            } else {

                if (std::tolower(*pIt) != std::tolower(*sIt)) {

                    return false;

                }
            }

            ++pIt;
            ++sIt;
        }
    }

    return pIt == pEndIt && sIt == sEndIt;
}

} // namespace internal


} // namespace fs
} // namespace oda
