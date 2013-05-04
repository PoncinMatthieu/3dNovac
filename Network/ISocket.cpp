
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        13/04/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Define.h"
#ifdef SYSTEM_WINDOWS
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
#endif
#include "SocketTcp.h"

using namespace Nc;
using namespace Nc::Network;

#ifdef SYSTEM_WINDOWS
const SOCKET     ISocket::_invalidDescriptor = INVALID_SOCKET;
#else
const int        ISocket::_invalidDescriptor = -1;
#endif

// window need some intitialization
#ifdef SYSTEM_WINDOWS
struct GlobalSocketInitializer
{
	GlobalSocketInitializer()
	{
		WSADATA data;
		WSAStartup(MAKEWORD(2,2), &data);
	}

	~GlobalSocketInitializer()
	{
		WSACleanup();
	}
};
static GlobalSocketInitializer	_globalSocketInitializer;
#endif

ISocket::ISocket()
    : _descriptor(_invalidDescriptor)
{
}

ISocket::~ISocket()
{
}

void        ISocket::SetBlocking(bool state)
{
#ifdef SYSTEM_WINDOWS
    u_long blocking = (state) ? 0 : 1;
    ioctlsocket(_descriptor, FIONBIO, &blocking);
#else
    int status = fcntl(_descriptor, F_GETFL);
    if (state)
        fcntl(_descriptor, F_SETFL, status & ~O_NONBLOCK);
    else
        fcntl(_descriptor, F_SETFL, status | O_NONBLOCK);
#endif
}

