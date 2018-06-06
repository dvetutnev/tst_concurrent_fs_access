/*
 * ODA filesystem. Errors.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"
#include <common/exception.h>

#include <boost/filesystem/convenience.hpp>


namespace oda {
namespace fs {


class Exception : public ::oda::error::Exception
{
    using ::oda::error::Exception::Exception;
};


namespace internal {


void reThrow(const boost::filesystem::filesystem_error&, const Path&);


} // namespace internal


} // namespace fs
} // namespace oda
