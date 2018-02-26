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
#include "command.h"
#include <iostream>
#include <stdio.h>

CommandValue::CommandValue(const std::string& v) : _key(v)
{
}

CommandValue::~CommandValue()
{
}

bool CommandValue::operator==(const char* v)
{
    return _key == v;
}
std::ostream& CommandValue::Print(std::ostream& os) const
{
    os << _key.c_str();
    return os;
}

std::ostream& operator<<(std::ostream& os, const CommandValue& obj)
{
    return obj.Print(os);
}



CommandValuePair::CommandValuePair(const std::string& v1,const std::string v2) : CommandValue(v1), _value(v2)
{
}

CommandValuePair::~CommandValuePair()
{
}

std::ostream& CommandValuePair::Print(std::ostream& os) const
{
    os << _key << " " << _value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const CommandValuePair& obj)
{
    return obj.Print(os);
}


Command::Command()
{
}
Command::~Command()
{
    Values::iterator itr = _internal.begin();
    for(; itr != _internal.end(); ++itr )
    {
	delete *itr;
    }
}

int Command::parse(const std::string& buf,Command& rtn)
{
    std::string::size_type s = 0;
    for( std::string::size_type t=buf.find('&',s);
	 t != std::string::npos;
	 t = buf.find('&',s))
    {
	std::string pair = buf.substr(s,t-s);
	s = t + 1;
	std::string::size_type p = pair.find('=');
	if( p != std::string::npos )
	{
	    rtn._internal.push_back( new CommandValuePair( pair.substr(0,p), pair.substr(p + 1) ) );
	}
	else
	{
	    rtn._internal.push_back( new CommandValue(pair) );
	}
    }
    if( s < buf.size() )
    {
	std::string pair = buf.substr(s);
	std::string::size_type p = pair.find('=');
	if( p != std::string::npos )
	{
	    rtn._internal.push_back( new CommandValuePair( pair.substr(0,p), pair.substr(p + 1) ) );
	}
	else
	{
	    rtn._internal.push_back( new CommandValue(pair) );
	}
    }
    return rtn._internal.size();
}

const CommandValue* Command::operator[](unsigned int i)
{
    if( _internal.size() < i )
    {
	return NULL;
    }
    return _internal[i];
}

const char* Command::operator[](const char* key)
{
    Values::const_iterator itr = _internal.begin();
    for(; itr != _internal.end(); ++itr )
    {
	if( *(*itr) == key )
	{
	    const CommandValuePair* pair = dynamic_cast<const CommandValuePair*>(*itr);
	    if( pair )
	    {
		return pair->value();
	    }
	    return key;
	}
    }
    return NULL;
}
