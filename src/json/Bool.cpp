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
