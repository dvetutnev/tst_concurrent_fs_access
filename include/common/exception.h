#pragma once


#include <system_error>


namespace oda {
namespace error {


class Exception : public std::system_error
{
    using system_error::system_error;
};


}   // namespace error
}   // namespace oda
