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
#include <string>
#include <vector>

class CommandValue
{
protected:
    std::string _key;
    virtual std::ostream& Print(std::ostream& os) const;
public:
    CommandValue(const std::string& v);
    virtual ~CommandValue();
    bool operator==(const char* v);
    
    friend std::ostream& operator<<(std::ostream& os, const CommandValue& obj);
};

class CommandValuePair : public virtual CommandValue
{
protected:
    std::string _value;
    virtual std::ostream& Print(std::ostream& os) const;
public:
    CommandValuePair(const std::string& v1,const std::string v2);
    virtual ~CommandValuePair();
    friend std::ostream& operator<<(std::ostream& os, const CommandValuePair& obj);
    inline const char* value() const { return _value.c_str(); } 
};

class Command
{
private:
    typedef std::vector<CommandValue*> Values;
    Values _internal;
public:
    Command();
    ~Command();

    static int parse(const std::string& buf,Command& rtn);
    const CommandValue* operator[](unsigned int i);
    const char* operator[](const char* key);    
    inline size_t size()
    {
	return _internal.size();
    }
};
