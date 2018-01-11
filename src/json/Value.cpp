/*
    This file is part of heos-cpp-api.

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
#include <iostream>

#include "Value.h"
#include "String.h"
#include "Number.h"
#include "Array.h"
#include "Object.h"
#include "Bool.h"

namespace json
{

Value::Value()
{
}

Value::~Value()
{
}

Value* Value::parse(uint8_t*& b, uint32_t& line)
{
    Value* rtn = NULL;
    skip(b, line);
    if (*b == '"')
    {
        rtn = String::parse(b, line);
    }
    else if ((*b >= '0' && *b <= '9') || *b == '-')
    {
        rtn = Number::parse(b, line);
    }
    else if (*b == '[')
    {
        rtn = Array::parse(b, line);
    }
    else if (*b == '{')
    {
        rtn = Object::parse(b, line);
    }
    else if( (rtn = Bool::parse(b, line)) != NULL )
    {

    }
    else
    {
        std::cerr << "Type mismatch on line: " << line << std::endl;
    }
    return rtn;
}

}// namespace json