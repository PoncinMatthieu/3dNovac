
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

    File Created At:        12/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifdef SYSTEM_WINDOWS
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
#endif
#include "../Define.h"
#include "SocketTcp.h"

using namespace Nc;
using namespace Nc::Network;

SocketTcp::SocketTcp()
{
}

SocketTcp::~SocketTcp()
{
}

void    SocketTcp::Close()
{
    if (IsValid())
    {
        #ifdef SYSTEM_WINDOWS
            if (closesocket(_descriptor) == -1)
        #else
            if (close(_descriptor) == -1)
        #endif
                LOG_ERROR << "Failed to close the socket" << std::endl;
        _descriptor = InvalidDescriptor();
    }
}

void    SocketTcp::Create()
{
    if (!IsValid())
    {
        _descriptor = socket(PF_INET, SOCK_STREAM, 0);
		if (!IsValid())
			throw Utils::Exception("SocketTcp", "Failed to create the socket tcp.");
		// To avoid the "Address already in use" error message when trying to bind to the same port
		int yep = 1;
		if (setsockopt(_descriptor, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&yep), sizeof(int)) == -1)
			LOG_ERROR << "Failed to set the socket option `SO_REUSEADDR`. Binding to a same port may fail if too fast" << std::endl;
		// Disable the Nagle algorithm (ie. removes buffering of TCP packets)
		if (setsockopt(_descriptor, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yep), sizeof(yep)) == -1)
			LOG_ERROR << "Failed to set socket option `TCP_NODELAY`. All your TCP packets will be buffered" << std::endl;
	}
}

bool    SocketTcp::Connect(const Ip &ip, unsigned short port)
{
    if (!IsValid())
        throw Utils::Exception("SocketTcp", "Can't connect, The socket is not valid");

    // Build the host address
    sockaddr_in s;
    memset(s.sin_zero, 0, sizeof(s.sin_zero));
    s.sin_addr.s_addr = inet_addr(ip.ToString().c_str());
    s.sin_family      = AF_INET;
    s.sin_port        = htons(port);

    if (connect(_descriptor, reinterpret_cast<sockaddr*>(&s), sizeof(s)) == -1)
        return false;
    return true;
}

bool    SocketTcp::Listen(unsigned short port)
{
    if (!IsValid())
        throw Utils::Exception("SocketTcp", "Can't listen, The socket is not valid");

    // Build the address
    sockaddr_in s;
    memset(s.sin_zero, 0, sizeof(s.sin_zero));
    s.sin_addr.s_addr = htonl(INADDR_ANY);
    s.sin_family      = AF_INET;
    s.sin_port        = htons(port);

    // Bind the socket to the specified port
    if (bind(_descriptor, reinterpret_cast<sockaddr*>(&s), sizeof(s)) == -1)
    {
        LOG_ERROR << "Failed to bind the socket to port " << port << std::endl;
        return false;
    }

    // Listen to the bound port
    if (listen(_descriptor, 0) == -1)
    {
        LOG_ERROR << "Failed to listen to port " << port << std::endl;
        return false;
    }
    return true;
}

bool    SocketTcp::Accept(SocketTcp &newClient, Ip &newclientIp)
{
    if (!IsValid())
        throw Utils::Exception("SocketTcp", "Can't accept, The socket is not valid");

    sockaddr_in clientAddr;
    #ifdef SYSTEM_WINDOWS
    int         len = sizeof(clientAddr);
    #else
    socklen_t   len = sizeof(clientAddr);
    #endif

    // Accept a new connection
    newClient._descriptor = accept(_descriptor, reinterpret_cast<sockaddr*>(&clientAddr), &len);
    if (!newClient.IsValid())
        return false;
    newclientIp.Init(inet_ntoa(clientAddr.sin_addr));
    return true;
}

void    SocketTcp::Write(const unsigned char src[], size_t size)
{
    if (!IsValid())
        throw Utils::Exception("SocketTcp", "Can't write, The socket is not valid");

    if (size > 0)
    {
        size_t sent = 0;
        for (size_t len = 0; len < size; len += sent)
        {
            sent = send(_descriptor, reinterpret_cast<const char*>(src) + len, size - len, 0);
            if (sent <= 0)
            {
                Close(); // pipe broken, closing socket.
                throw Utils::Exception("SocketTcp", "Failed to write over the socket.");
            }
        }
    }
}

size_t    SocketTcp::Read(unsigned char dst[], size_t size)
{
    if (!IsValid())
        throw Utils::Exception("SocketTcp", "Can't read, The socket is not valid");
    size_t r = 0;
    if (size > 0)
    {
        r = recv(_descriptor, dst, static_cast<int>(size), 0);
        if (r <= 0)
            Close(); // pipe closed, closing socket.
    }
    return r;
}


