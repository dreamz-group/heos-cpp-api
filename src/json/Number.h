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