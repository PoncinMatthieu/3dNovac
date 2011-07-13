
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

#include "../Define.h"
#ifdef SYSTEM_WINDOWS
#else
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

#include "Ip.h"

using namespace Nc;
using namespace Nc::Network;

const Ip     Ip::LocalHost("127.0.0.1");

Ip::Ip()
    : _addr(INADDR_NONE)
{
}

Ip::Ip(const std::string &ip)
{
    Init(ip);
}

Ip::Ip(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4)
{
    Init(byte1, byte2, byte3, byte4);
}

void    Ip::Init(const std::string &ip)
{
    // convertion in ipv4
    _addr = inet_addr(ip.c_str());

    // if we can't convert the address, try to resolve it
    if (_addr == INADDR_NONE)
    {
        hostent *h = gethostbyname(ip.c_str());
        _addr = (h) ? reinterpret_cast<in_addr*>(h->h_addr)->s_addr : INADDR_NONE;
    }
}

void    Ip::Init(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4)
{
    _addr = htonl((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4);
}

std::string     Ip::ToString() const
{
    in_addr InAddr;
    InAddr.s_addr = _addr;
    return inet_ntoa(InAddr);
}
