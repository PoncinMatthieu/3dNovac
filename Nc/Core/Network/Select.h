
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

#ifndef NC_CORE_NETWORK_SELECT_H_
#define NC_CORE_NETWORK_SELECT_H_

#include "../Define.h"
#include "ISocket.h"

namespace Nc
{
	/// To Abstract some usefull class to manipulate network connections
    namespace Network
    {
        /// Define a select class used to determine which socket can be read or write.
		/**
			<div class="title">Full sample to use a select in a Tcp client application:</div>

 \code
int main()
{
	char				buf[2048 + 1];
	Network::Ip			ip("127.0.0.1");
	Network::SocketTcp	socket;
	Network::Select		select;

	// connection
	socket.Create();
	if (!socket.Connect(ip, 4242))
		throw Utils::Exception("Can't connect to the given ip");

	// read on the socket until a deconnection
	while (socket.IsValid())
	{
		// clear the select and set the socket for reading accept
		// those things needs to be done at each loop before a Wait
		select.Clear();
		select.SetForReading(socket);

		// Wait for incoming data
		if (select.Wait(0.5f) > 0)
		{
			// if the socket is ready for read, then read
			if (select.IsReadyForReading(socket))
			{
				int r = socket.Read(buf, 2048);

				// disconnected ?
				if (r <= 0)
					socket.Close();
				else
				{
					// print the incoming data
					buf[r] = '\0';
					LOG << buf << std::endl;
				}
			}
		}
	}
	return 0;
}
 \endcode

		*/
        class LCORE Select
        {
            public:
                Select();

                /** Clear the set of read and write. */
                void            Clear();
                /** Set the given \p socket for reading */
                void            SetForReading(const ISocket &socket);
                /** Set the given \p socket for writing */
                void            SetForWriting(const ISocket &socket);

                /** Wait until one of the sockets is ready for reading/writing, or \p timeout is reached */
                unsigned int    Wait(float timeout = 0.f);

                /** \return true if the given \p socket is ready for reading */
                bool            IsReadyForReading(const ISocket &socket);
                /** \return true if the given \p socket is ready for writing */
                bool            IsReadyForWriting(const ISocket &socket);

            private:
                fd_set                  _readSet;           ///< Set of socket to watch
                fd_set                  _writeSet;          ///< Set of socket to write
                int                     _max;               ///< Maximum socket index
        };
    }
}

#endif
