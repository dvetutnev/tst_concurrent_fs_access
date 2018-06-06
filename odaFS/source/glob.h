/*
 * ODA filesystem. Check filepaths by mask.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/directory.h"


namespace oda {
namespace fs {
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


} // namespace internal
} // namespace fs
} // namespace oda
