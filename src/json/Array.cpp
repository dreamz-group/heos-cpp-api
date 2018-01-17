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
#include <sstream>

#include "Array.h"

namespace json
{


Array::Array()
{
}

Array::~Array()
{
    json::Array::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        delete *itr;
    }
}

type_t Array::getType() const
{
    return ARRAY;
}

Value* Array::parse(uint8_t*& b, uint32_t& line)
{
    skip(b, line);
    if (*b != '[')
    {
        std::cerr << "Parse error missing [ at line:" << line << std::endl;
        return NULL;
    }
    ++b;
    skip(b, line);
    Array* arr = new Array();
    if (*b == ']')
    {
        ++b;
        return arr;
    }

    bool more = true;
    do
    {
        Value* rtn = Value::parse(b, line);
        if (rtn == NULL)
        {
            delete arr;
            return NULL;
        }
        arr->_items.push_back(rtn);
        skip(b, line);
        if (*b != ',')
        {
            more = false;
        }
        else
        {
            ++b;
        }
    } while (more);

    if (*b != ']')
    {
        std::cerr << "Parse error missing ] at line:" << line << std::endl;
        delete arr;
        return NULL;
    }
    ++b;
    return arr;
}

std::string Array::str() const
{
    std::stringstream sstr;
    sstr << this;
    return sstr.str();
}

std::ostream& operator<<(std::ostream& os, const Array* arr)
{
    os << "[" << std::endl;
    json::Array::VALUES::const_iterator itr = arr->_items.begin();
    while (itr != arr->_items.end())
    {
        os << *itr;
        ++itr;
        if (itr != arr->_items.end())
        {
            os << "," << std::endl;
        }
        else
        {
            os << std::endl;
        }
    }

    os << "]";
    return os;
}
}// namespace json