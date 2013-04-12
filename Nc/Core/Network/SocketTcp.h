
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
        /// Define a Tcp Socket.
		/**
			To use a socket TCP you will need to `Create` it before doing anything.
            And a created socket need to be closed with `Close`.

			To create a client, use the method `Connect`,
			and to create a listening socket to accept connection you'll need to use the `Listen` method to initialize it
			and `Accept` to accept a new connection.

			\sa
				- Ip
				- Select
		*/
        class LCORE SocketTcp : public ISocket
        {
            public:
                SocketTcp();
                ~SocketTcp();

                /** Create the socket descriptor. */
                virtual void    Create();
                /** Close the descriptor. */
                virtual void    Close();

                /**
                    Connect the socket to the given \p ip and the given \p port.
                    \return true if no error.
                */
                bool            Connect(const Ip &ip, unsigned short port);

                /**
                    Set the descriptor to listening a port for connection or incomming data.
                    \return true if no error.
                */
                bool            Listen(unsigned short port);

                /**
                    Wait until a new incomming connection. Fill the new socket and ip used to communicate with the client.
                    \return true if no error.
                */
                bool            Accept(SocketTcp &newClient, Ip &newclientIp);

                /**
                    Write (send) to the descriptor an array of type T.
                    Close the socket automatically if the pipe is broken.
                */
                template<typename T, unsigned int D>
                void            WriteDatagram(const Math::Array<T,D> &src);
                /**
                    Write (send) to the descriptor an array of byte.
                    Close the socket automatically if the pipe is broken.
                */
                void            Write(const unsigned char src[], size_t size);

                /**
                    Read (receive) from the descriptor an array of type T into \p dst.
                    Close the socket automatically if the pipe is closed.
                    \return the received size. If the received size is 0 and you send more than 0 byte, it meens that the connection has been disconnected.
                */
                template<typename T, unsigned int D>
                size_t          ReadDatagram(Math::Array<T,D> &dst);
                /**
                    Read (receive) from the descriptor an array of byte into \p dst.
                    Close the socket automatically if the pipe is closed.
                    \return the received size. If the received size is 0 and you send more than 0 byte, it meens that the connection has been disconnected.
                */
                size_t          Read(unsigned char dst[], size_t size);
        };

        template<typename T, unsigned int D>
        void            SocketTcp::WriteDatagram(const Math::Array<T,D> &src)
        {
            Write((const unsigned char*)src.data, src.Size());
		}

		template<typename T, unsigned int D>
		size_t          SocketTcp::ReadDatagram(Math::Array<T,D> &dst)
		{
            return Read((unsigned char*)dst.data, dst.Size());
        }
    }
}

#endif
