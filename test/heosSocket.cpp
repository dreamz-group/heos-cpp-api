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

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <assert.h>

#include "json.h"
#include "heosSocket.h"
#include "command.h"

int main(int argc,char* argv[])
{
    uint8_t buf[2048];
    uint32_t pId = 943328861; // Player ID.
    const char* addr = "192.168.1.219";
    heosSocket socket;
    printf("Init\n");
    if( socket.Init(addr) != 0 )
    {
        fprintf(stderr,"Failed to connect to device %s Error message '%s'\n",addr, socket.error);
	return -1;
    }    
    /*    printf("PlayStream\n");
    socket.PlayStream("http://50.7.77.114:8101/listen.pls",pId);
    if( socket.Recv(sizeof(buf),buf) < 0 )
    {
	fprintf(stderr,"Failed to start stream %s\n",socket.error);
	return -1;
    }
    socket.Print(buf);
    */
    printf("Basic Commands\n\n");
    for( uint8_t i=0; heosSocket::basicCommand[i] != NULL; ++i )
    {
	if( socket.BasicCommand(i) < 0 )
	{
	    fprintf(stderr,"Failed test on command: %d '%s'\n",i,heosSocket::basicCommand[i]);
	    break;
	}
	if( socket.Recv(sizeof(buf),buf) < 0 )
	{
	    fprintf(stderr,"Failed test (recive) on command: %d '%s'\n",i,heosSocket::basicCommand[i]);
	    break;
	}
	printf("Command: %s\n", heosSocket::basicCommand[i]);
	socket.Print(buf);
    }
    /*
    printf("Player Commands\n\n");
    for( uint8_t i=0; heosSocket::playerCommand[i] != NULL; ++i )
    {
	if( socket.PlayerCommand(i,pId) < 0 )
	{
	    fprintf(stderr,"Failed test on command: %d '%s'\n",i,heosSocket::playerCommand[i]);
	    break;
	}
	if( socket.Recv(sizeof(buf),buf) < 0 )
	{
	    fprintf(stderr,"Failed test (recive) on command: %d '%s'\n",i,heosSocket::playerCommand[i]);
	    break;
	}
	printf("Command: %s\n",heosSocket::playerCommand[i]);
	socket.Print(buf);
    }
*/
    printf("SetPlayMode\n");
    if( socket.SetPlayMode( RepeatMode::all, ShuffleMode::on, pId ) < 0 )
    {
	fprintf(stderr,"Failed test SetPlayMode error '%s'\n", socket.error );
	return -1;
    }
    
    if( socket.Recv(sizeof(buf),buf) < 0 )
    {
	fprintf(stderr,"Failed test (recive) message '%s'\n",socket.error);
	return -1;
    }
    socket.Print(buf);
    
    json::Parse p;
    json::Value* v = p.read(buf);
    if( v == NULL )
    {
	fprintf(stderr,"Failed to parse responce %s", buf );
	delete v;
	return -1;
    }
    
    assert( v->getType() == json::OBJECT );
    json::Object* o    = dynamic_cast<json::Object*>(v);
    json::Object* heos = dynamic_cast<json::Object*>((*o)["heos"]);
    assert( heos != NULL );

    json::Value* rv = (*heos)["result"];
    assert( rv->getType() == json::STRING );
    json::String* result = dynamic_cast<json::String*>(rv);

    if( *result != "success" )
    {
	fprintf(stderr,"status code on success!? %s", result->c_str() );
	delete v;
	return -1;
    }

    json::Value* mv = (*heos)["message"];
    assert( mv->getType() == json::STRING );
    json::String* message = dynamic_cast<json::String*>(mv);
    Command values;
    if( Command::parse(message->str(),values) <= 0 )
    {
	fprintf(stderr,"Command::parse faild to parse string '%s'", message->str().c_str() );
	delete v;
	return -1;
    }
    for( int i=0; i < values.size(); ++i )
    {
	std::cout << *values[i] << std::endl;
    }

    if( strcmp( values["repeat"],RepeatMode::all ) != 0 )
    {
	fprintf(stderr,"Unexpected repeat mode expected:%s recived%s\n", RepeatMode::all, values["repeat"] );
    }
    
    if( strcmp( values["shuffle"],ShuffleMode::on ) != 0 )	
    {
	fprintf(stderr,"Unexpected shuffle mode expected:%s recived%s\n", ShuffleMode::on, values["shuffle"] );
    }
    delete v;
    socket.Close();
}
