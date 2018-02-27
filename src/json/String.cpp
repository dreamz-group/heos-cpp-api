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

#include "String.h"

namespace json
{


String::String()
{
}

String::String(const char* value)
{
    _value = value;
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
           (*b >= '#' && *b <= '~'))
    {
        if( *b == '\\')
        {            
            id += *(char*)b; ++b;   
        }
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
