
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

#ifndef NC_CORE_NETWORK_SOCKETUDP_H_
#define NC_CORE_NETWORK_SOCKETUDP_H_

#ifdef SYSTEM_WINDOWS
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
#endif

#include "ISocket.h"

namespace Nc
{
    namespace Network
    {
        /// Define an Udp Socket
        class LCORE SocketUdp : public ISocket
        {
            public:
                SocketUdp();
                ~SocketUdp();

                /** Create the socket descriptor */
                virtual void    Create();
                /** Close the descriptor */
                virtual void    Close();

                /**
                    Bind the socket to the given port
                    \return true if no error
                */
                bool            Bind(unsigned int port);
                /**
                    Unbind the socket to its previous port
                    \return true if no error
                */
                bool            Unbind();

                /**
                    Write (send) to the descriptor an array of type T to the ip and port specified
                    \return true if no error
                */
                template<typename T, unsigned int D>
                bool            Write(const Math::Array<T,D> &src, const Ip &ip, unsigned int port);
                /**
                    Write (send) to the descriptor an array of byte to the ip and port specified
                    \return true if no error
                */
                bool            Write(const char *src, unsigned int size, const Ip &ip, unsigned int port);

                /**
                    Read (receive) from the descriptor an array of type T into \p dst.
                    \return the received size, and fill the ip/port of the sender
                */
                template<typename T, unsigned int D>
                int             Read(Math::Array<T,D> &dst, Ip &ip, unsigned int &port);
                /**
                    Read (receive) from the descriptor an array of byte into \p dst.
                    \return the received size, and fill the ip/port of the sender
                */
                int             Read(char *dst, unsigned int maxSize, Ip &ip, unsigned int &port);

            private:
                unsigned int            _bindedPort;
        };

        template<typename T, unsigned int D>
        bool            SocketUdp::Write(const Math::Array<T,D> &src, const Ip &ip, unsigned int port)
        {
            if (!IsValid())
                throw Utils::Exception("SocketUdp", "Can't write, The socket is not valid");

            if (src.Size() > 0)
            {
                // Build the target address
                sockaddr_in target;
                target.sin_family      = AF_INET;
                target.sin_port        = htons(port);
                target.sin_addr.s_addr = inet_addr(ip.ToString().c_str());
                memset(target.sin_zero, 0, sizeof(target.sin_zero));

                // Loop until every byte has been sent
                int sent = 0;
                int reelSize = src.Size() * sizeof(T);
                for (int len = 0; len < reelSize; len += sent)
                {
                    sent = sendto(_descriptor, reinterpret_cast<const char*>(src.Data) + len, reelSize - len, 0, reinterpret_cast<sockaddr*>(&target), sizeof(target));
                    if (sent <= 0)
                        return false;
                }
            }
            return true;
		}

		template<typename T, unsigned int D>
		int    SocketUdp::Read(Math::Array<T,D> &dst, Ip &ip, unsigned int &port)
		{
            if (!IsValid())
                throw Utils::Exception("SocketUdp", "Can't read, The socket is not valid");
            if (_bindedPort == 0)
                throw Utils::Exception("SocketUdp", "Can't read, The socket must be bound to a port");

            int r = 0;
            if (dst.Size() > 0)
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
                r = recvfrom(_descriptor, dst.Data, static_cast<int>(dst.Size() * sizeof(T)), 0, reinterpret_cast<sockaddr*>(&sender), &senderSize);

                // Check the number of bytes received and fill the ip/port of the sender
                if (r > 0)
                {
                    ip = Ip(inet_ntoa(sender.sin_addr));
                    port = ntohs(sender.sin_port);
                    r /= sizeof(T);
                }
                else
                {
                    ip = Ip();
                    port = 0;
                }
            }
            return r;
        }
    }
}

#endif
