/*
    This file is part of heos-cpp-api.

    MIT License
    Copyright (c) 2018 dreamz-group

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include <assert.h>
#include <math.h>
#include <sstream>

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

std::string Number::str() const
{
    std::stringstream sstr;
    sstr << this;
    return sstr.str();
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
