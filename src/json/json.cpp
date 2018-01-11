/*
    This file is part of heos-cpp-api.

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

#include <iostream>

#include "json.h"

namespace json
{

Parse::Parse()
{
}

Value* Parse::read(uint8_t* b)
{
    uint32_t line = 1;
    return Object::parse(b, line);
}

std::ostream& operator<<(std::ostream& os, const json::Value* obj)
{
    if ( obj == NULL )
    {
        os << "null";
        return os;
    }
    switch (obj->getType())
    {
    case json::OBJECT:
        os << dynamic_cast<const json::Object*>(obj);
        break;
    case json::ARRAY:
        os << dynamic_cast<const json::Array*>(obj);
        break;
    case json::NUMBER:
        os << dynamic_cast<const json::Number*>(obj);
        break;
    case json::STRING:
        os << dynamic_cast<const json::String*>(obj);
        break;
    default:
        os << "Invalid object " << obj->getType() << std::endl;
        break;
    }
    return os;
}

} // namespace json

