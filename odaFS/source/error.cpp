/*
 * ODA filesystem. Errors.
 * Dmitriy Vetutnev, ODANT 2018
*/


#include "fs/error.h"


namespace oda {
namespace fs {


namespace internal {


void reThrow(const boost::filesystem::filesystem_error& ex, const Path& path) {

    const boost::system::error_code& boostEc = ex.code();
#ifdef _WIN32
    const std::error_code stdEc{boostEc.value(), std::system_category()};
#else
    const std::error_code stdEc{boostEc.value(), std::generic_category()};
#endif
    throw Exception{stdEc, path.string()};
}


} // namespace internal


} // namespace fs
} // namespace oda
