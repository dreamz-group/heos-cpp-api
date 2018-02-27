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
#ifndef __STRING_H__
#define __STRING_H__

#include "Value.h"

namespace json
{

class String : public virtual Value
{
    std::string _value;
public:
    String();
    String(const char* value);
    virtual ~String();
    virtual type_t getType() const;
    static Value* parse(uint8_t*& b, uint32_t& line);
    static bool parse_string(std::string& id, uint8_t*& b, uint32_t& line);
    inline bool operator==(const char* str) { return _value == str; }
    inline bool operator!=(const char* str) { return _value != str; }
    inline const char*        c_str() { return _value.c_str(); }
    virtual std::string str() const { return _value; }

private:
    friend std::ostream& operator<<(std::ostream& os, const String* obj);
};
} // namespace json
#endif // __STRING_H__
