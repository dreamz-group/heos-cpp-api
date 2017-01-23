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
//std::ostream& operator<<(std::ostream& os, const json::Value& obj);

#endif // __JSON_H__
