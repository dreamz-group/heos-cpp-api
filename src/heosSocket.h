/*! \File heosSocker.h
    This file is part of heos-c-api.

    heos-c-api is free software: you can redistribute it and/or modify
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

class heosSocket
{
private:
  int _socket;                        //! Internal socket.
  int Write(uint8_t* buf,size_t len); //! Internal function for writing to socket.
  
public:
  static __thread char error[1024];   //! Thread local error message.
  static const char* basicCommand[];  //! List of basic commands.  
  static const char* playerCommand[]; //! List of commands for players.
  
  heosSocket();
  ~heosSocket();
  
  int Init(const char* addrIp);                       //! Init the connection.
  int BasicCommand(uint8_t commandId);                //! Send Basic Command.
  int PlayerCommand(uint8_t commandId, uint32_t pid); //! Send a player command.

  int PlayStream(const char* url, uint32_t pid);      //! Play a specific url.
  int Recv(uint32_t size, uint8_t* buf);              //! Recive status from a command.

  void Close();                                       //! Close the connection.

  int Print( uint8_t* buf );                          //! Print a responce buffer.

};
