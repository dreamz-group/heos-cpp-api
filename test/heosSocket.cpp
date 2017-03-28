/*
    This file is part of heos-c-api.

    heos-cpp-api is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    heos-c-api is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with heos-c-api.  If not, see <http://www.gnu.org/licenses/>.
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

#include "json.h"
#include "heosSocket.h"

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
    printf("PlayStream\n");
    socket.PlayStream("http://50.7.77.114:8101/listen.pls",pId);
    if( socket.Recv(sizeof(buf),buf) < 0 )
    {
	fprintf(stderr,"Failed to start stream %s\n",socket.error);
	return -1;
    }
    socket.Print(buf);
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
    socket.Close();
}
