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