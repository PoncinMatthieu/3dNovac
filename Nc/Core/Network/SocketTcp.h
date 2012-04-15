
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

#ifndef NC_CORE_NETWORK_SOCKETTCP_H_
#define NC_CORE_NETWORK_SOCKETTCP_H_

#include "ISocket.h"

namespace Nc
{
    namespace Network
    {
        /// Define a Tcp Socket
		/**
			To use a socket you will need to `Create` it before doing anything.
			A created socket need to be closed with `Close`.
			To create a client, use the method `Connect`,
			and to create a listening socket to accept connection you'll need to use the `Listen` method to initialize it
			and `Accept` to accept a new connection.

			\sa
				- Network::Ip
				- Network::Select
		*/
        class SocketTcp : public ISocket
        {
            public:
                SocketTcp();
                ~SocketTcp();

                /** Create the socket descriptor */
                virtual void    Create();
                /** Close the descriptor */
                virtual void    Close();

                /**
                    Connect the socket to the given \p ip and the given \p port
                    \return true if no error.
                */
                bool            Connect(const Ip &ip, unsigned int port);

                /**
                    Set the descriptor to listening a port for connection or incomming data
                    \return true if no error.
                */
                bool            Listen(unsigned int port);

                /**
                    Wait until a new incomming connection. Fill the new socket and ip used to communicate with the client.
                    \return true if no error.
                */
                bool            Accept(SocketTcp &newClient, Ip &newclientIp);

                /**
                    Write (send) to the descriptor an array of type T
                    \return true if no error
                */
                template<typename T, unsigned int D>
                bool            Write(const Math::Array<T,D> &src);
                /**
                    Write (send) to the descriptor an array of byte
                    \return true if no error
                */
                bool            Write(const char *src, unsigned int size);

                /**
                    Read (receive) from the descriptor an array of type T into \p dst.
                    \return the received size. If the received size is 0 and you send more than 0 byte, it meens that the connection has been disconnected.
                */
                template<typename T, unsigned int D>
                int             Read(Math::Array<T,D> &dst);
                /**
                    Read (receive) from the descriptor an array of byte into \p dst.
                    \return the received size. If the received size is 0 and you send more than 0 byte, it meens that the connection has been disconnected.
                */
                int             Read(char *dst, unsigned int maxSize);
        };

        template<typename T, unsigned int D>
        bool            SocketTcp::Write(const Math::Array<T,D> &src)
        {
            if (!IsValid())
                throw Utils::Exception("SocketTcp", "Can't write, The socket is not valid");

            if (src.Size() > 0)
            {
                int sent = 0;
                int reelSize = src.Size() * sizeof(T);
                for (int len = 0; len < reelSize; len += sent)
                {
					sent = send(_descriptor, static_cast<const char*>(src.Data) + len, reelSize - len, 0);
					if (sent <= 0)
						return false;
				}
			}
			return true;
		}

		template<typename T, unsigned int D>
		int             SocketTcp::Read(Math::Array<T,D> &dst)
		{
			if (!IsValid())
				throw Utils::Exception("SocketTcp", "Can't read, The socket is not valid");
			int r = 0;
			if (dst.Size() > 0)
			{
				r = recv(_descriptor, static_cast<char*>(dst.Data), static_cast<int>(dst.Size() * sizeof(T)), 0);
                if (r != 0)
                    r /= sizeof(T);
			}
            return r;
        }
    }
}

#endif
