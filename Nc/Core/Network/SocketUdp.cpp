
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
#include "SocketUdp.h"

using namespace Nc;
using namespace Nc::Network;

SocketUdp::SocketUdp()
{
}

SocketUdp::~SocketUdp()
{
}

void    SocketUdp::Close()
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
        _bindedPort = 0;
    }
}

void    SocketUdp::Create()
{
    if (!IsValid())
    {
        _bindedPort = 0;
        _descriptor = socket(AF_INET, SOCK_DGRAM, 0);
		if (!IsValid())
			throw Utils::Exception("SocketUdp", "Failed to create the socket udp.");
		// To avoid the "Address already in use" error message when trying to bind to the same port
		int yep = 1;
		if (setsockopt(_descriptor, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&yep), sizeof(int)) == -1)
			LOG_ERROR << "Failed to set the socket option `SO_REUSEADDR`. Binding to a same port may fail if too fast" << std::endl;
        // Enable broadcast by default
        if (setsockopt(_descriptor, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yep), sizeof(int)) == -1)
            LOG_ERROR << "Failed to enable broadcast on UDP socket" << std::endl;
	}
}

bool    SocketUdp::Bind(unsigned short port)
{
    if (!IsValid())
        throw Utils::Exception("SocketUdp", "Can't bind, The socket is not valid");

    // Check if the socket is already bound to the specified port
    if (_bindedPort != port)
    {
        // If the socket was previously bound to another port, we need to unbind it first
        Unbind();

        if (port != 0)
        {
            // Build an address with the specified port
            sockaddr_in addr;
            addr.sin_family      = AF_INET;
            addr.sin_port        = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

            // Bind the socket to the port
            if (bind(_descriptor, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
            {
                LOG_ERROR << "Failed to bind the socket to port " << port << std::endl;
                _bindedPort = 0;
                return false;
            }
        }

        // Save the new port
        _bindedPort = port;
    }
    return true;
}

bool    SocketUdp::Unbind()
{
    if (!IsValid())
        throw Utils::Exception("SocketUdp", "Can't unbind, The socket is not valid");

    // recreate the socket
    if (_bindedPort != 0)
    {
        Close();
        Create();
        _bindedPort = 0;
    }
    return true;
}

bool    SocketUdp::Write(const char *src, unsigned int size, const Ip &ip, unsigned short port)
{
    if (!IsValid())
        throw Utils::Exception("SocketUdp", "Can't write, The socket is not valid");

    if (size > 0)
    {
        // Build the target address
        sockaddr_in target;
        target.sin_family      = AF_INET;
        target.sin_port        = htons(port);
        target.sin_addr.s_addr = inet_addr(ip.ToString().c_str());
        memset(target.sin_zero, 0, sizeof(target.sin_zero));

        // Loop until every byte has been sent
        int sent = 0;
        for (unsigned int len = 0; len < size; len += sent)
        {
            sent = sendto(_descriptor, reinterpret_cast<const char*>(src) + len, size - len, 0, reinterpret_cast<sockaddr*>(&target), sizeof(target));
            if (sent <= 0)
                return false;
        }
    }
    return true;
}

int    SocketUdp::Read(char *dst, unsigned int maxSize, Ip &ip, unsigned short &port)
{
    if (!IsValid())
        throw Utils::Exception("SocketUdp", "Can't read, The socket is not valid");
    if (_bindedPort == 0)
        throw Utils::Exception("SocketUdp", "Can't read, The socket must be bound to a port");

    int r = 0;
    if (maxSize > 0)
    {
        // Data that will be filled with the other computer's address
        sockaddr_in sender;
        sender.sin_family      = AF_INET;
        sender.sin_port        = 0;
        sender.sin_addr.s_addr = INADDR_ANY;
        memset(sender.sin_zero, 0, sizeof(sender.sin_zero));
        #ifdef SYSTEM_WINDOWS
        int senderSize = sizeof(sender);
        #else
        socklen_t senderSize = sizeof(sender);
        #endif

        // Receive a chunk of bytes
        r = recvfrom(_descriptor, dst, static_cast<int>(maxSize), 0, reinterpret_cast<sockaddr*>(&sender), &senderSize);

        // Check the number of bytes received and fill the ip/port of the sender
        if (r > 0)
        {
            ip = Ip(inet_ntoa(sender.sin_addr));
            port = ntohs(sender.sin_port);
        }
        else
        {
            ip = Ip();
            port = 0;
        }
    }
    return r;
}
