/*
 * ODA filesystem. Remove operations.
 * Dmitriy Vetutnev, ODANT 2018
*/


#pragma once


#include <fs/common.h>


namespace oda {
namespace fs {


void remove(const Path&);
void removeAll(const Path&);


} // namespace fs
} // namespace oda
