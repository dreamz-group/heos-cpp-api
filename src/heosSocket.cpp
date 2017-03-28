/*! \File heosSocket.cpp
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

#include "heosSocket.h"
#include "json.h"

__thread char heosSocket::error[1024] = {0};

const char* heosSocket::basicCommand[] =
{
    "system/heart_beat",  //! Send heart beat command.
    "player/get_players", //! Get avalible players.
    NULL
};
  
const char* heosSocket::playerCommand[] =
{
    "player/get_player_info",
    "player/get_play_state",
    "player/get_now_playing_media",
    "player/get_volume",
    "player/get_mute",
    "player/get_play_mode",
    /*"player/toggle_mute",
     "player/play_next",
     "player/play_previous",*/
    NULL
};

heosSocket::heosSocket()
{
    _socket = -1;
}

heosSocket::~heosSocket()
{
    if( _socket != -1 )
    {
	close(_socket);
	_socket = -1;
    }
}

int heosSocket::Init(const char* ipAddr)
{
    if( _socket == -1 )
    {
	_socket = socket( AF_INET, SOCK_STREAM, 0);
    }
    if( _socket < 0 )
    {
	snprintf(error,sizeof(error),"Failed to open socket %s\n", strerror(errno));
	_socket = -1;
	return -1;
    }
    
    int one = 1;    
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    struct  sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr( ipAddr );
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons( 1255 );

    socklen_t addrLen = sizeof(addr);    
    int status = connect(_socket, (struct sockaddr*)&addr, addrLen);
    if( status )
    {
	snprintf(error,sizeof(error),"Failed to connect:%s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
    return 0;
}

int heosSocket::Write(uint8_t* buf, size_t len)
{
    ssize_t rtn = write(_socket,buf,len);
    if( rtn < 0 )
    {       
	snprintf(error,sizeof(error),"Failed to write to socket: %s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
    
    if( rtn != len )
    {
	snprintf(error,sizeof(error),"Failed to write compeate message: %s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
    return len;
}

int heosSocket::BasicCommand(uint8_t commandId)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://%s\r\n",basicCommand[commandId]);
    return Write( buf, len );
}

int heosSocket::PlayerCommand(uint8_t commandId, uint32_t pid)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://%s?pid=%d\r\n",playerCommand[commandId], pid);
    return Write( buf, len );
}

int heosSocket::PlayStream(const char* url, uint32_t pid)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://browser/play_stream?pid=%d&url=%s\r\n",pid,url);
    return Write( buf, len );
}

int heosSocket::Recv(uint32_t size, uint8_t* buf)
{
    int timeout = 1;
    int pos = 0;
    while (1) 
    {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(_socket, &rfds);

	struct timeval tv;
	tv.tv_sec  = timeout;
	tv.tv_usec = 0;
	
	int rtn = select(_socket+1, &rfds, NULL, NULL, &tv);
	
	if (rtn < 0)
	{
	    snprintf(error,sizeof(error),"select failed on socket: %d error: %s", _socket, strerror(errno));
	    return -1;
	}

	if( rtn == 0 ) 
	{
	    snprintf(error,sizeof(error),"No data within %d ms.", timeout);
	    break;
	}

	if (FD_ISSET(_socket, &rfds) )
	{
	    size_t tsize = size - pos;
	    int read = recv(_socket, &buf[pos], tsize, MSG_DONTWAIT);
	    if( read <= 0 )
	    {
		break;
	    }
	    pos += read;
	    if( pos > 2 && buf[pos-2]== '\r' && buf[pos-1] == '\n')
	    {
		return pos;
	    }
		
	}
    }
    return -1;
}

void heosSocket::Close()
{
    if( _socket < 0 )
	return;
    
    close( _socket );
    _socket = -1;
}

int heosSocket::Print( uint8_t* buf )
{
    json::Parse p;
    json::Value* v = p.read(buf);
    if( v == NULL )
    {
	snprintf(error,sizeof(error),"Failed to parse responce: %s\n",buf);
	return -1;
    }
    std::cout << "--------------------------------------" << std::endl
	      << v << std::endl
	      << "--------------------------------------" << std::endl
	      << std::endl;
    delete v;
    return 0;
}

