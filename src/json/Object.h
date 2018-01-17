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
#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <vector>

#include "Value.h"

namespace json
{

class Object : public virtual Value
{
public:
    Object();
    virtual ~Object();

    virtual type_t getType() const;
    virtual std::string str() const;

    static Value* parse(uint8_t*& b, uint32_t& line);
    // Give a key as index for values.
    Value* operator[](const char* key);
    // Give integer as index to values.
    Value* operator[](unsigned int index);

    // Give integer as index to get a key;
    const std::string& key(unsigned int index);

    void Add(const std::string& name, Value* value);

protected:
    typedef std::pair<std::string, Value*> VALUE;
    typedef std::vector<VALUE> VALUES;

    VALUES _items;
    bool value_pair(Object* obj, uint8_t*& b, uint32_t& line, bool& more);

private:
    friend std::ostream& operator<<(std::ostream& os, const Object* obj);
};
} // namespace json
#endif // __OBJECT_H__