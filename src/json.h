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

#ifndef __JSON_H__
#define __JSON_H__
#include <iostream>
#include <vector>
#include <stdint.h>

namespace json
{
    typedef enum
    {
	NDEF,
	OBJECT,
	ARRAY,
	STRING,
	NUMBER
    } type_t;
    
    class Value
    {
    public:
	Value();
	virtual ~Value();
	virtual type_t getType() const = 0;
	friend std::ostream& operator<<(std::ostream& os, const Value* obj);
	static inline void skip(uint8_t*& b,uint32_t& line)
	{
	    while( *b == ' ' || *b == '\t' || *b == '\n' )
	    {
		if( *b == '\n' )
		    ++line;
		++b;
	    }
	}
	static Value* parse( uint8_t*& b, uint32_t& line );
    };

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
	static Value* parse(uint8_t*& b, uint32_t& line);
    private:
	friend std::ostream& operator<<(std::ostream& os, const Number* obj);
	static inline bool digit( uint8_t*& b, uint64_t& value )
	{
	    if( *b < '0' || *b > '9')
	    {
		return false;
	    }
	    while( *b >= '0' && *b <= '9')
	    {
		value = (value * 10) + (*b - '0');
		++b;
	    }
	    return true;
	}
    };
	
    class String : public virtual Value
    {
	std::string _value;
    public:
	String();
	virtual ~String();
	virtual type_t getType() const;
	static Value* parse(uint8_t*& b, uint32_t& line);
	static bool parse_string( std::string& id, uint8_t*& b, uint32_t& line);
	inline bool operator==(const char* str) { return _value == str; }
	inline bool operator!=(const char* str) { return _value != str; }
	inline const char*        c_str() { return _value.c_str(); }
	inline const std::string& str()   { return _value; }

    private:
	friend std::ostream& operator<<(std::ostream& os, const String* obj);
    };


    class Object : public virtual Value
    {
    public:
	Object();
	virtual ~Object();

	virtual type_t getType() const;

	static Value* parse(uint8_t*& b, uint32_t& line );
	Value* operator[](const char* key);
	
    protected:
	typedef std::pair<std::string, Value*> VALUE;
	typedef std::vector<VALUE> VALUES;

	VALUES _items;
	bool value_pair( Object* obj, uint8_t*& b, uint32_t& line, bool& more );

    private:
	friend std::ostream& operator<<(std::ostream& os, const Object* obj);
    };
    
    class Array : public virtual Value
    {
    public:
	Array();
	virtual ~Array();
	virtual type_t getType() const;
	static Value* parse(uint8_t*& b, uint32_t& line );
    protected:
	typedef std::vector<Value*> VALUES;
	VALUES _items;
	
    private:
	friend std::ostream& operator<<(std::ostream& os, const Array* obj);
    };
    

    class Parse
    {
    public:
	Parse();

	void parse_number(uint8_t*& b, uint32_t& line);
	void parse_array(uint8_t*& b, uint32_t& line);
	void parse_object(uint8_t*& b, uint32_t& line);
	Value* read(uint8_t* b);
    };   
}

#endif // __JSON_H__
