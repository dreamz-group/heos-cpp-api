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
#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>
#include <iostream>
namespace json
{

typedef enum
{
    NDEF,
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOL
} type_t;

class Value
{
public:
    Value();
    virtual ~Value();
    virtual type_t getType() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Value* obj);
    static inline void skip(uint8_t*& b, uint32_t& line)
    {
        while (*b == ' ' || *b == '\t' || *b == '\n' || *b == '\r')
        {
            if (*b == '\n')
                ++line;
            ++b;
        }
    }
    static Value* parse(uint8_t*& b, uint32_t& line);
    virtual std::string str() const = 0;    
};
} // namespace json
#endif // __NUMBER_H__