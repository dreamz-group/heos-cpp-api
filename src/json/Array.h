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
#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <vector>
#include "Value.h"

namespace json
{

class Array : public virtual Value
{
public:
    Array();
    virtual ~Array();
    virtual type_t getType() const;
    static Value* parse(uint8_t*& b, uint32_t& line);
    virtual std::string str() const;

protected:
    typedef std::vector<Value*> VALUES;
    VALUES _items;

private:
    friend std::ostream& operator<<(std::ostream& os, const Array* obj);
};
} // namespace json
#endif // __ARRAY_H__