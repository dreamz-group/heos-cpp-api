#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <malloc.h>
#include <math.h>

#include "json.h"

namespace json
{

Value::Value()
{
}

Value::~Value()
{
}

Value* Value::parse( uint8_t*& b, uint32_t& line )
{
    Value* rtn = NULL;
    skip( b, line);
    if( *b  == '"' )
    {
	rtn = String::parse(b,line);
    }
    else if( *b >= '0' && *b <= '9' )
    {
	rtn = Number::parse(b,line);
    }
    else if( *b == '[' )
    {
	rtn = Array::parse(b,line);
    }
    else if( *b == '{' )
    {
	rtn = Object::parse(b,line);
    }
    else
    {
	fprintf(stderr,"Type mismatch on line:%d\n",line);
    }
    return rtn;
}

Number::Number(uint64_t v)
{
    _type = UINT64;
    _value.ui = v;
}

Number::Number(int64_t v)
{
    _type = INT64;
    _value.i = v;
}

Number::Number(double v)
{
    _type = DOUBLE;
    _value.d = v;
}
  
Number::~Number()
{
}
  
type_t Number::getType() const
{
    return NUMBER;
}
    
Value* Number::parse(uint8_t*& b, uint32_t& line)
{
    uint64_t value   = 0;
    bool     dec     = false;
    bool     neg     = false;
    double   dbl     = 0;
    if( *b == '-' )
    {
	neg = true;
	++b;
    }
    
    if( !digit(b,value) )
    {
	fprintf(stderr,"Not a number at line:%d\n", line );
	return NULL;
    }

    if( *b == '.' )
    {
	dbl = (double)value;
	uint8_t* t = b;
	dec = true;
	++b;
	if( !digit(b,value) )
	{
	    fprintf(stderr,"Must be atleast one digit after '.' at line:%d\n", line);
	    return NULL;
	}
	dbl += (double)value / pow(10, b-t);
    }
    if( *b == 'e' || *b == 'E' )
    {
	fprintf(stderr,"Number in format x.yEn not implemented at line:%d\n", line);
	return NULL;
    }
    if( dec )
    {
	return new Number(neg ? -dbl : dbl);
    }
    return neg ? new Number( - (int64_t)value ) : new Number( value );
}

std::ostream& operator<<(std::ostream& os, const Number* obj)
{
    switch( obj->_type )
    {
    case Number::UINT64:
	os << obj->_value.ui;
	break;
    case Number::INT64:
	os << obj->_value.i;
	break;
    case Number::DOUBLE:
	os << obj->_value.d;
	break;
    default:
	os << "Invalid number type:" << obj->_type << std::endl;
        break;
    }
    return os;
}
  
String::String()
{
}
  
String::~String()
{
}
  
type_t String::getType() const
{
    return STRING;
}
	
Value* String::parse(uint8_t*& b, uint32_t& line)
{
    String* rtn = new String();
    if( parse_string( rtn->_value, b, line) )
    {
	return rtn;
    }
    delete rtn;
    return NULL;	    
}
	
bool String::parse_string( std::string& id, uint8_t*& b, uint32_t& line)
{
    if( *b != '\"' )
    {
	fprintf(stderr,"Parse error missing start \" of id at line:%u\n", line );
	return false;
    }
    ++b;
    id = "";

    while( (*b == ' ' ) ||
	   (*b == '!' ) ||
	   (*b >= '#' && *b <= '[') ||
	   (*b >= ']' && *b <= '~') )
    {
	id += *(char*)b;
		++b;
    }
    if( *b != '\"' )
    {
	fprintf(stderr,"Parse error missing end \" of id at line:%u on char '%c' \n", line, *b );
	return false;
    }
    ++b;
    return true;
}

std::ostream& operator<<(std::ostream& os, const String* obj)
{
    os << "\"" << obj->_value << "\"";
    return os;
}

Array::Array()
{
}

Array::~Array()
{
    json::Array::VALUES::iterator itr = _items.begin();
    for(; itr != _items.end(); ++itr )
    {
	delete *itr;
    }
}

type_t Array::getType() const
{
    return ARRAY;
}

Value* Array::parse(uint8_t*& b, uint32_t& line )
{
    skip(b,line);
    if( *b != '[' )
    {		
	fprintf(stderr,"Parse error missing [ at line:%u\n", line );
	return NULL;
    }
    ++b;
    Array* arr = new Array();
    if( *b == ']' )
    {
	++b;
	return arr;
    }
    
    bool more = true;
    do
    {
	Value* rtn = Value::parse(b,line);
	if( rtn == NULL )
	{
	    delete arr;
	    return NULL;
	}	
	arr->_items.push_back( rtn );
	skip(b,line);
	if( *b != ',' )
	{
	    more = false;
	}
	else
	{
	    ++b;
	}
    }while( more );
    
    if( *b != ']' )
    {
	fprintf(stderr,"Parse error missing ] at line:%u\n", line );
	delete arr;
	return NULL;
    }
    ++b;
    return arr;
}

std::ostream& operator<<(std::ostream& os, const Array* arr)
{
    os << "[" << std::endl;
    json::Array::VALUES::const_iterator itr = arr->_items.begin();
    while( itr != arr->_items.end() )
    {
        os << *itr;
	++itr;
	if( itr != arr->_items.end() )
	{
	    os << "," << std::endl;
	}
	else
	{
	    os << std::endl;
	}
    }
    
    os << "]";
    return os;
}

  
Object::Object()
{
}

Object::~Object()
{
    json::Object::VALUES::iterator itr = _items.begin();
    for( ; itr != _items.end(); ++itr )
    {
	delete itr->second;
    }
}

type_t Object::getType() const
{
    return OBJECT;
}
    
bool Object::value_pair( Object* obj, uint8_t*& b, uint32_t& line, bool& more )
{
    // Parse id
    skip( b, line );
    std::string id;
    if( !String::parse_string(id,b,line) )
    {
	return false;
    }
    skip( b, line );
    if( *b != ':' )
    {
	fprintf(stderr,"Parse error missing : at line:%u\n", line );
	return false;
    }
    ++b;
    
    Value* rtn = Value::parse(b,line);
    // TODO bool, null
    if( rtn == NULL )
    {
	return false;
    }
    obj->_items.push_back( VALUE(id,rtn) );
    skip(b,line);
    if( *b != ',' )
    {
	more = false;
    }
    else
    {
	++b;
    }
    return true;
}

Value* Object::parse(uint8_t*& b, uint32_t& line )
{
    skip(b,line);
    if( *b != '{' )
    {		
	fprintf(stderr,"Parse error missing { at line:%u\n", line );
	return NULL;
    }
    ++b;
    Object* obj = new Object();
    if( *b == '}' )
    {
	++b;
	return obj;
    }
    bool more = true;
    while( more )
    {
	if( !obj->value_pair( obj, b, line, more ) )
	{
	    return NULL;
	}
    }
    if( *b != '}' )
    {
	fprintf(stderr,"Parse error missing } at line:%u\n", line );
	delete obj;
	return NULL;
    }
    ++b;
    return obj;
}

std::ostream& operator<<(std::ostream& os, const Object* obj)
{
    os << "{" << std::endl;
    json::Object::VALUES::const_iterator itr = obj->_items.begin();
    while( itr != obj->_items.end() )
    {
	os << "\"" << itr->first << "\" : " << itr->second;
	++itr;
	if( itr != obj->_items.end() )
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

    

Parse::Parse()
{
}

Value* Parse::read(uint8_t* b)
{
    uint32_t line = 1;
    return Object::parse(b,line);
}

std::ostream& operator<<(std::ostream& os, const json::Value* obj)
{
    switch( obj->getType() )
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

/*
#include <string.h>

int main( int argc,char* argv[])
{
    json::Parse p;
    FILE* fd = fopen(argv[1],"r");
    if( fd == NULL )
    {
	fprintf(stderr,"Failed to read file %s",argv[1]);
	return -1;
    }
    uint8_t* buffer = (uint8_t*)malloc(1024);
    memset(buffer,0,1024);
    fread (buffer,1,1024,fd);
     
    printf("-----------------\n%s\n------------------\n",buffer);
    json::Value* v = p.read(buffer);
    free(buffer);
    
    if( v == NULL )
    {
	std::cout << "ERROR!" << std::endl;
	return -1;
    }
    std::cout << (const json::Value*)v << std::endl;
    delete v;
    return 0;
}
*/
