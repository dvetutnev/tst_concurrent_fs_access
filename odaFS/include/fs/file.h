/*
 * ODA filesystem. Files IO operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include "fs/common.h"
#include "fs/error.h"


namespace oda {
namespace fs {


std::string readFile(const Path&);

void writeFile(const Path&, const std::string&, std::ios_base::openmode);


} // namespace fs
} // namespace oda
