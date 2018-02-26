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
#include <sstream>

#include "Object.h"
#include "String.h"

namespace json
{

Object::Object()
{
}

Object::~Object()
{
    json::Object::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        delete itr->second;
    }
}

type_t Object::getType() const
{
    return OBJECT;
}

void Object::Add(const std::string& name, Value* value)
{
    _items.push_back(VALUE(name, value));
}

bool Object::value_pair(Object* obj, uint8_t*& b, uint32_t& line, bool& more)
{
    // Parse id
    skip(b, line);
    std::string id;
    if (!String::parse_string(id, b, line))
    {
        return false;
    }
    skip(b, line);
    if (*b != ':')
    {
        std::cerr << "Parse error missing : at line:" << line << std::endl;
        return false;
    }
    ++b;

    Value* rtn = Value::parse(b, line);
    // TODO bool, null
    if (rtn == NULL)
    {
        return false;
    }
    obj->_items.push_back(VALUE(id, rtn));
    skip(b, line);
    if (*b != ',')
    {
        more = false;
    }
    else
    {
        ++b;
    }
    return true;
}

Value* Object::parse(uint8_t*& b, uint32_t& line)
{
    skip(b, line);
    if (*b != '{')
    {
        std::cerr << "Parse error missing { at line:" << line << std::endl;
        return NULL;
    }
    ++b;
    skip(b, line);
    Object* obj = new Object();
    if (*b == '}')
    {
        ++b;
        return obj;
    }
    bool more = true;
    while (more)
    {
        if (!obj->value_pair(obj, b, line, more))
        {
            delete obj;
            return NULL;
        }
    }
    if (*b != '}')
    {
        std::cerr << "Parse error missing } at line:" << line << std::endl;
        delete obj;
        return NULL;
    }
    ++b;
    return obj;
}

std::string Object::str() const
{
    std::stringstream sstr;
    sstr << this;
    return sstr.str();
}

std::ostream& operator<<(std::ostream& os, const Object* obj)
{
    os << "{" << std::endl;
    json::Object::VALUES::const_iterator itr = obj->_items.begin();
    while (itr != obj->_items.end())
    {
        os << "\"" << itr->first << "\" : " << itr->second;
        ++itr;
        if (itr != obj->_items.end())
        {
            os << "," << std::endl;
        }
        else
        {
            os << std::endl;
        }
    }

    os << "}";
    return os;
}

Value* Object::operator[](const char* key)
{
    json::Object::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        if (itr->first == key)
        {
            return itr->second;
        }
    }
    return NULL;
}

bool Object::key(unsigned int index, std::string& out)
{
    if( _items.size() > index )
    {
        VALUE& v = _items[index];
        out = v.first;
        return true;
    }
    return false;
}

Value* Object::operator[](unsigned int index)
{
    if( _items.size() > index )
    {
        VALUE v = _items[index];
        return v.second;
    }
    return NULL;
}

}// namespace json