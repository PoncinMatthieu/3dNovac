
#ifndef NC_NET_CALLBACK_H_
#define NC_NET_CALLBACK_H_

#include <Nc/Core/Utils/NonCopyable.h>
#include "Define.h"
#include "Packet.h"
#include "Layer.h"
#include "ReliableLayer.h"
#include "FlowControl.h"

namespace Nc
{
    namespace Net
    {
        /// Base class receiving every requests from an udp/tcp connections
        class LIB_NC_NET Callback : Utils::NonCopyable
        {
            public:
                typedef void (Callback::*Method)(const Packet &p);

                Callback(TcpConnection *tcpConnection = NULL, UdpConnection *udpConnection = NULL);
                virtual ~Callback();

                /** Convert the callback to string. */
                friend std::ostream	&operator << (std::ostream &oss, const Callback &c);

                /** \return a string describing the callback (eg: client id) */
                virtual void            ToString(std::ostream &oss) const       {}

                /** \return the tcp handler. */
                TcpConnection		*GetTcpConnection()		{return _tcpConnection;}
                /** \return the tcp handler. */
                const TcpConnection	*GetTcpConnection() const	{return _tcpConnection;}
                /** \return the udp handler. */
                UdpConnection		*GetUdpConnection()		{return _udpConnection;}
                /** \return the udp handler. */
                const UdpConnection	*GetUdpConnection() const	{return _udpConnection;}
                /** Set the connection used by the callback. */
                virtual void		SetupConnections(TcpConnection *tcpConnection, UdpConnection *udpConnection = NULL);

                /** Call the given callback, can be redefined, to for exemple manage a ping method and check if clients are responding. */
                virtual void    CallCallback(Method callback, const Packet &p);

                /** Update the flow control, check acked packets and send states using the flow control class. */
                virtual void    Update(float elapsedTime);

                /** \return the tcp layer taking care of the protocol. */
                Layer           &TcpLayer()                             {return _tcpLayer;}
                /** \return the udp layer taking care of the protocol. */
                ReliableLayer   &UdpLayer()                             {return _udpLayer;}

                /** Called to check lost packets from the udp layer. */
                virtual void    CheckLostPackets(const PacketQueue &lostPackets)    {}
                /** Called to send the game state to the udp connection. */
                virtual void    SendStates()                                        {}

                /** \return the time to the next update states on the client. */
                float           NextUpdateStatesTime() const            {return (1.f / _flowControl.SendRate()) - _lastUpdateStatesTime;}

            protected:
                TcpConnection   *_tcpConnection;            ///< handler for the tcp connection.
                UdpConnection   *_udpConnection;            ///< handler for the udp connection.

                Layer           _tcpLayer;                  ///< manage the tcp protocol application layer.
                ReliableLayer   _udpLayer;                  ///< manage the udp protocol application layer.
                FlowControl     _flowControl;               ///< class controlling the send rate to send to the client for the udp connection.
                bool            _sendStates;                ///< if true, check acked packets and send states depending on the flow control.

            private:
                float           _lastUpdateStatesTime;
        };
    }
}

#endif
