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
