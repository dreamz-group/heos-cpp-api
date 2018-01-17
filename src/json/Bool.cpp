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
#include "Bool.h"

namespace json
{

Bool::Bool()
{
}

Bool::Bool(bool b)
{
    _value = b;
}

Bool::~Bool()
{
}

type_t Bool::getType() const
{
    return BOOL;
}

Value* Bool::parse(uint8_t*& b, uint32_t& line)
{
    skip(b,line);
    if ((*b     == 'T' || *b     == 't') &&
        (*(b+1) == 'R' || *(b+1) == 'r') &&
        (*(b+2) == 'U' || *(b+2) == 'u') &&
        (*(b+3) == 'E' || *(b+3) == 'e'))
    {
        b+=4;
        return new Bool(true);
    }
    else if ((*b     == 'F' || *b     == 'f') &&
             (*(b+1) == 'A' || *(b+1) == 'a') &&
             (*(b+2) == 'L' || *(b+2) == 'l') &&
             (*(b+3) == 'S' || *(b+3) == 's') &&
             (*(b+4) == 'E' || *(b+4) == 'e'))
    {
        b+=5;
        return new Bool(false);
    }
    return NULL;
}

std::string Bool::str() const
{
    return std::string(_value ? "true" : "false");
}

std::ostream& operator<<(std::ostream& os, const Bool* obj)
{
    os << (obj->_value ? "true" : "false");
    return os;
}

}// namespace json