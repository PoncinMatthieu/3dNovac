
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

#include "Select.h"

using namespace Nc;
using namespace Nc::Network;

Select::Select()
{
    Clear();
}

void Select::Clear()
{
    FD_ZERO(&_readSet);
    FD_ZERO(&_writeSet);
    _max = 0;
}

void Select::SetForReading(const SocketTcp &socket)
{
    if (socket.IsValid())
    {
        FD_SET(socket.Descriptor(), &_readSet);
        int size = static_cast<int>(socket.Descriptor());
        if (size > _max)
            _max = size;
    }
}

void Select::SetForWriting(const SocketTcp &socket)
{
    if (socket.IsValid())
    {
        FD_SET(socket.Descriptor(), &_writeSet);
        int size = static_cast<int>(socket.Descriptor());
        if (size > _max)
            _max = size;
    }
}

bool Select::IsReadyForReading(const SocketTcp &socket)
{
    return (FD_ISSET(socket.Descriptor(), &_readSet));
}

bool Select::IsReadyForWriting(const SocketTcp &socket)
{
    return (FD_ISSET(socket.Descriptor(), &_writeSet));
}

unsigned int Select::Wait(float timeout)
{
    if (timeout > 0)
    {
        timeval t;
        t.tv_sec  = static_cast<long>(timeout);
        t.tv_usec = (static_cast<long>(timeout * 1000) % 1000) * 1000;
		return select(_max + 1, &_readSet, &_writeSet, NULL, &t);
	}
	else
		return select(_max + 1, &_readSet, &_writeSet, NULL, NULL);
}
