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
#include <assert.h>

#include "String.h"

namespace json
{


String::String()
{
}

String::~String()
{
}

type_t String::getType() const
{
    return STRING;
}

Value* String::parse(uint8_t*& b, uint32_t& line)
{
    String* rtn = new String();
    if (parse_string(rtn->_value, b, line))
    {
        return rtn;
    }
    delete rtn;
    return NULL;
}

bool String::parse_string(std::string& id, uint8_t*& b, uint32_t& line)
{
    if (*b != '\"')
    {
        std::cerr << "Parse error missing start \" of id at line:" << line << std::endl;
        return false;
    }
    ++b;
    id = "";

    while ((*b == ' ') ||
        (*b == '!') ||
        (*b >= '#' && *b <= '[') ||
        (*b >= ']' && *b <= '~'))
    {
        id += *(char*)b;
        ++b;
    }
    if (*b != '\"')
    {
        std::cerr << "Parse error missing end \" of id at line:" << line << " on char '" << *(char*)b << "'" << std::endl;
        return false;
    }
    ++b;
    return true;
}

std::ostream& operator<<(std::ostream& os, const String* obj)
{
    os << "\"" << obj->_value << "\"";
    return os;
}


}// namespace json