/*
 * ODA filesystem. Rename operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <fs/common.h>


namespace oda {
namespace fs {


void rename(const Path&, const Path&);


} // namespace fs
} // namespace oda
