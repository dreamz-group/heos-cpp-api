/*
    This file is part of heos-cpp-api

    heos-cpp-api is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    heos-cpp-api is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with heos-cpp-api.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __JSON_H__
#define __JSON_H__

#include "Value.h"
#include "Number.h"
#include "String.h"
#include "Bool.h"
#include "Object.h"
#include "Array.h"

namespace json
{

class Parse
{
public:
    Parse();

    void parse_number(uint8_t*& b, uint32_t& line);
    void parse_array(uint8_t*& b, uint32_t& line);
    void parse_object(uint8_t*& b, uint32_t& line);
    Value* read(uint8_t* b);
};
} //namespace json

#endif // __JSON_H__
