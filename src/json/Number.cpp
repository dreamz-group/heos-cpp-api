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
#include <math.h>

#include "Number.h"

namespace json
{


Number::Number(uint64_t v)
{
    _type = UINT64;
    _value.ui = v;
}

Number::Number(int64_t v)
{
    _type = INT64;
    _value.i = v;
}

Number::Number(double v)
{
    _type = DOUBLE;
    _value.d = v;
}

Number::~Number()
{
}

uint64_t Number::value()
{
    assert( _type == UINT64 );
    return _value.ui;
}

type_t Number::getType() const
{
    return NUMBER;
}

Value* Number::parse(uint8_t*& b, uint32_t& line)
{
    uint64_t value = 0;
    bool     dec = false;
    bool     neg = false;
    double   dbl = 0;
    if (*b == '-')
    {
        neg = true;
        ++b;
    }

    if (!digit(b, value))
    {
        std::cerr << "Not a number at line:" << line << std::endl;
        return NULL;
    }

    if (*b == '.')
    {
        dbl = (double)value;
        uint8_t* t = b;
        dec = true;
        ++b;
        if (!digit(b, value))
        {
            std::cerr << "Must be atleast one digit after '.' at line:" << line << std::endl;
            return NULL;
        }
        dbl += (double)value / pow(10, b - t);
    }
    if (*b == 'e' || *b == 'E')
    {
        std::cerr << "Number in format x.yEn not implemented at line:" << line << std::endl;
        return NULL;
    }
    if (dec)
    {
        return new Number(neg ? -dbl : dbl);
    }
    return neg ? new Number(-(int64_t)value) : new Number(value);
}

std::ostream& operator<<(std::ostream& os, const Number* obj)
{
    switch (obj->_type)
    {
    case Number::UINT64:
        os << obj->_value.ui;
        break;
    case Number::INT64:
        os << obj->_value.i;
        break;
    case Number::DOUBLE:
        os << obj->_value.d;
        break;
    default:
        os << "Invalid number type:" << obj->_type << std::endl;
        break;
    }
    return os;
}


}// namespace json