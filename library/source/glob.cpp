/*
 * ODA filesystem. Check filepaths by mask.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "glob.h"

#include <boost/locale/conversion.hpp>


namespace oda {
namespace fs {
namespace internal {


Glob::Glob(const Path& path, CaseSensitive caseSensitive)
    :
    _caseSensitive{ (caseSensitive == CaseSensitive::True) ? true : false }
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
} // namespace fs
} // namespace oda
