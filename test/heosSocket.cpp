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

static __thread int _socket = -1; 
static const char* basicCommand[] = {"system/heart_beat",
			             "player/get_players",
				     NULL
                                    };

static const char* playerCommand[] = {"player/get_player_info",
				      "player/get_play_state",
				      "player/get_now_playing_media",
				      "player/get_volume",
				      "player/get_mute",
				      "player/toggle_mute",
				      "player/get_play_mode",
				      "player/play_next",
				      "player/play_previous",
				      NULL
                                     };
int heosSocketInit()
{
    if( _socket == -1 )
    {
	_socket = socket( AF_INET, SOCK_STREAM, 0);
    }
    if( _socket < 0 )
    {
	fprintf(stderr,"Failed to open socket %s\n", strerror(errno));
	_socket = -1;
	return -1;
    }
    
    int one = 1;    
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    struct  sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr( "192.168.1.219" );
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons( 1255 );

    socklen_t addrLen = sizeof(addr);    
    int status = connect(_socket, (struct sockaddr*)&addr, addrLen);
    if( status )
    {
	fprintf(stderr,"Failed to connect:%s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
}

int heosSocketWrite(uint8_t* buf,size_t len)
{
    ssize_t rtn = write(_socket,buf,len);
    if( rtn < 0 )
    {
	fprintf(stderr,"Failed to write to socket: %s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
    
    if( rtn != len )
    {
	fprintf(stderr,"Failed to write compeate message: %s",strerror(errno));
	close(_socket);
	_socket = -1;
	return -1;
    }
    return len;
}

int heosSocketBasicCommand(uint8_t commandId)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://%s\r\n",basicCommand[commandId]);
    return heosSocketWrite( buf, len );
}

int heosSocketPlayerCommand(uint8_t commandId, uint32_t pid)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://%s?pid=%d\r\n",playerCommand[commandId], pid);
    return heosSocketWrite( buf, len );
}

int heosSockerPlayStream(const char* url, uint32_t pid)
{
    uint8_t buf[1024];
    size_t len = snprintf((char*)buf,sizeof(buf),"heos://browser/play_stream?pid=%d&url=%s\r\n",pid,url);
    return heosSocketWrite( buf, len );
}

int heosSocketRecv(uint32_t size, uint8_t* buf)
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
	    fprintf(stderr,"select failed on socket: %d error: %s", _socket, strerror(errno));
	    return -1;
	}

	if( rtn == 0 ) 
	{
	    fprintf(stderr,"No data within %d ms.", timeout);
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

void heosClose()
{
    if( _socket < 0 )
	return;
    close( _socket );
    _socket = -1;
}

int print( uint8_t* buf )
{
    json::Parse p;
    json::Value* v = p.read(buf);
    if( v == NULL )
    {
	fprintf(stderr,"Failed to parse responce: %s\n",buf);
	return -1;
    }
    std::cout << v;
    delete v;
    return 0;
}

int main(int argc,char* argv[])
{
    uint8_t buf[2048];
    uint32_t pId = 943328861; // Player ID.
    heosSocketInit();
    heosSockerPlayStream("http://50.7.77.114:8101/listen.pls",pId);
    if( heosSocketRecv(sizeof(buf),buf) < 0 )
    {
	fprintf(stderr,"Failed to start stream %s\n",buf);
	return -1;
    }
    print(buf);
    for( uint8_t i=0; basicCommand[i] != NULL; ++i )
    {
	if( heosSocketBasicCommand(i) < 0 )
	{
	    fprintf(stderr,"Failed test on command: %d '%s'\n",i,basicCommand[i]);
	    break;
	}
	if( heosSocketRecv(sizeof(buf),buf) < 0 )
	{
	    fprintf(stderr,"Failed test (recive) on command: %d '%s'\n",i,basicCommand[i]);
	    break;
	}
	printf("%s\n",buf);
	print(buf);
    }
    heosClose();
    return 0;
    
    for( uint8_t i=0; playerCommand[i] != NULL; ++i )
    {
	if( heosSocketPlayerCommand(i,pId) < 0 )
	{
	    fprintf(stderr,"Failed test on command: %d '%s'\n",i,playerCommand[i]);
	    break;
	}
	if( heosSocketRecv(sizeof(buf),buf) < 0 )
	{
	    fprintf(stderr,"Failed test (recive) on command: %d '%s'\n",i,playerCommand[i]);
	    break;
	}
	printf("%s",buf);
    }    
    heosClose();
}
