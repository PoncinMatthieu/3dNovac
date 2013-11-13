
/*-----------------------------------------------------------------------------

	3dNovac Net
	Copyright (C) 2010-2013, The 3dNovac Team

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

    File Created At:        25/06/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_NET_UDPCONNECTION_H_
#define NC_NET_UDPCONNECTION_H_

#include <Nc/Core/Network/SocketUdp.h>
#include "Define.h"
#include "Connection.h"
#include "Callback.h"

namespace Nc
{
    namespace Net
    {
        class LIB_NC_NET UdpConnection : public Connection<Network::SocketUdp>
        {
            protected:
                struct CallbackMethod
                {
                    bool                mostRecentOnly;
                    int                 currentSequence;
                    Callback::Method    method;
                };
                typedef std::map<unsigned short, CallbackMethod>        MapCallbackMethod;
                typedef std::list<Nc::Math::Array<unsigned char> >      ListPendingRequest;

                struct Sender
                {
                    Nc::Network::Ip     ip;             ///< ip of the client.
                    unsigned short      port;           ///< port of the client.
                    Callback            *instance;      ///< instance of the callback for the client.
                    MapCallbackMethod   methods;        ///< methods of the callback for the client.
                    ListPendingRequest  pendingRequest; ///< pending request.
                };
                typedef std::list<Sender>       ListSender;

            public:
                UdpConnection();
                UdpConnection(unsigned short startingBoundPort);
                ~UdpConnection();

                /** Update the application layer. */
                void            Update(float elapsedTime);

                /** Bind the connection. */
                void            Bind(unsigned short startingBoundPort);
                /** \return binded port. */
                unsigned short  BoundPort() const                   {return _socket.BoundPort();}

                /** Send a Ack packet to establish a connection with the server. */
                void            Connect(Callback *sender);
                /** Close the socket. */
                void            Disconnect();

                /**
                    Push a new request to send over Udp.
                    \return the packet identifier to identify acked packets. -1 if an error occured.
                */
                int             PushRequest(unsigned char *packet, unsigned short packetSize, Callback *sender);
                /**
                    Push a new request to send over Udp.
                    \return the packet identifier to identify acked packets. -1 if an error occured.
                */
                int             PushRequest(unsigned char *packet, unsigned short code, unsigned short packetSize, Callback *sender);
                /**
                    Push an ack to send over Udp.
                    \return the packet identifier to identify acked packets. -1 if an error occured.
                */
                int             PushAck(Callback *sender);
                /** \return true if there is a pending request. */
                bool            HasPendingRequest() const           {return _hasPendingRequests;}

                /** Set ip and port of a callback instance. */
                void            AddCallbackInstance(Callback *instance, const Nc::Network::Ip &ip, unsigned short port);
                /** Remove the given callback instance. */
                void            RemoveCallbackInstance(Callback *instance);
                /** Add a new callback associated to a command code. */
                void            AddNewCallback(unsigned short code, bool mostRecentOnly, Callback *instance, Callback::Method method);

                /** Set read/write on the socket. */
                void            InitSelect(Nc::Network::Select &select);
                /** Call read/write if ready. */
                void            CheckIO(Nc::Network::Select &select);

            private:
                /** Send every pending requests. */
                void            SendRequests();
                /** Receive every requests. */
                void            ReceiveRequests();

                /**
                    Manage received packets.
                    \return the size of the packet.
                */
                unsigned int    ManageReceivedPacket(unsigned char *buffer, unsigned short readSize, const Nc::Network::Ip &ip, unsigned short port);

            protected:
                Nc::Network::Ip             _senderIp;              ///< sender ip of the last packet received.
                unsigned short              _senderPort;            ///< sender port of the last packet received.

                ListSender                  _clients;               ///< map of client.
                bool                        _hasPendingRequests;    ///< true if threre is a pending request.
        };
    }
}

#endif
