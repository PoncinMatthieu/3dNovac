
#ifndef NC_NET_TCPCONNECTION_H_
#define NC_NET_TCPCONNECTION_H_

#include <Nc/Core/Network/SocketTcp.h>
#include "Define.h"
#include "Connection.h"
#include "Callback.h"

namespace Nc
{
    namespace Net
    {
        /// Manage a Tcp connection.
        class LIB_NC_NET TcpConnection : public Connection<Network::SocketTcp>
        {
            protected:
                typedef std::list<Math::Array<unsigned char> >		ListPendingRequest;
                typedef std::map<unsigned short, Callback::Method>      MapCallback;

            public:
                TcpConnection();
                TcpConnection(const Network::Ip &ip, unsigned short port);
                ~TcpConnection();

                /** Set the callback instance. */
                void            SetCallbackInstance(Callback *instance) {_callbackInstance = instance;}
                /** \return the callback instance. */
                Callback        *GetCallbackInstance()                  {return _callbackInstance;}
                /** Remove every callbacks */
                void            ClearCallbacks()                        {_callbacks.clear();}
                /** Add a new callback associated to a command code. */
                void            AddNewCallback(unsigned short code, Callback::Method method);

                /** \return true if the Tcp connection with the GameServer is establish. */
                bool            Connected() const                       {return _socket.IsValid();}
                /** Close the Tcp socket connected with the GameServer. */
                void            Disconnect();
                /** Attempt to connect to the GameServer and send a CONNECTION packet. */
                virtual void    Connect();

                /** Create the socket to listen on the given port. */
                void            Listen(Nc::UInt16 port);
                /** Accept new connection. */
                bool            Accept(TcpConnection &newConnection);

                /** Set the ip of the client */
                void                    Ip(const Nc::Network::Ip &ip)   {_ip = ip;}
                /** \return the ip of the client */
                const Nc::Network::Ip   &Ip() const                     {return _ip;}
                /** Set the port of the client */
                void                    Port(unsigned short port)       {_port = port;}
                /** \return the port of the client */
                unsigned short          Port() const                    {return _port;}

                /** \return true if the client has a pending request. */
                bool            HasPendingRequests()                 {return !_pendingRequests.empty();}
                /** push a new request to send over Tcp. */
                void            PushRequest(unsigned char *packet, unsigned short code, unsigned short packetSize);

                /** Set read/write on the socket. */
                void            InitSelect(Network::Select &select);
                /** Call read/write if ready. */
                void            CheckIO(Network::Select &select);

            protected:
                /** Send every pending requests. */
                virtual void    SendRequests();
                /** Receive every requests. */
                virtual void    ReceiveRequests();
                /**
                    Called once a new connection is accepted succesfully (new client connection).
                    \return true if the connection is validated.
                */
                virtual bool    Accepted() {return true;}

                /**
                    Manage received packets.
                    \return the size of the packet.
                */
                unsigned int        ManageReceivedPacket(unsigned char *buffer, unsigned int readSize);

            protected:
                unsigned short              _port;      ///< port of the remote peer.
                Network::Ip		    _ip;        ///< ip of the remote peer.

                MapCallback                 _callbacks;                 ///< map of callbacks.
                ListPendingRequest          _pendingRequests;           ///< list of pending request to send over the connection.
                Callback                    *_callbackInstance;         ///< instance of the callback base class.
        };
    }
}

#endif
