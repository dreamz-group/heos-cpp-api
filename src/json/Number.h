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
#ifndef __NUMBER_H__
#define __NUMBER_H__

#include "Value.h"

namespace json
{

class Number : public virtual Value
{
    typedef enum
    {
        UINT64,
        INT64,
        DOUBLE
    } Type_t;
    Type_t _type;

    union
    {
        int64_t  i;
        uint64_t ui;
        double   d;
    } _value;
public:
    Number(uint64_t v);
    Number(int64_t v);
    Number(double v);
    virtual ~Number();
    virtual type_t getType() const;
    virtual std::string str() const;
    static Value* parse(uint8_t*& b, uint32_t& line);
    uint64_t value();
private:
    friend std::ostream& operator<<(std::ostream& os, const Number* obj);
    static inline bool digit(uint8_t*& b, uint64_t& value)
    {
        if (*b < '0' || *b > '9')
        {
            return false;
        }
        while (*b >= '0' && *b <= '9')
        {
            value = (value * 10) + (*b - '0');
            ++b;
        }
        return true;
    }
};
} // namespace json
#endif // __NUMBER_H__
