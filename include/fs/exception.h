/*
 * ODA Filesystem exceptions
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <common/exception.h>


namespace oda {
namespace fs {


class Exception : public ::oda::error::Exception
{
    using ::oda::error::Exception::Exception;
};


} // namespace fs
} // namespace oda
