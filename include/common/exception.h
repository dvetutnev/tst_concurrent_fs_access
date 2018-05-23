/*
 * Base class for ODA exceptions
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <system_error>


namespace oda {
namespace error {


class Exception : public std::system_error
{
    using std::system_error::system_error;
};


} // namespace error
} // namespace oda
