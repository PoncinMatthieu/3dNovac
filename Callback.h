
#ifndef NC_NET_CALLBACK_H_
#define NC_NET_CALLBACK_H_

#include <Nc/Core/Utils/NonCopyable.h>
#include "Define.h"
#include "Packet.h"
#include "Layer.h"

namespace Nc
{
    namespace Net
    {
        /// Base class receiving every requests from an udp/tcp connections
        class Callback : Utils::NonCopyable
        {
            public:
                typedef void (Callback::*Method)(const Packet &p);

                Callback(TcpConnection *tcpConnection = NULL);
                virtual ~Callback();

                /** Convert the callback to string. */
                friend std::ostream	&operator << (std::ostream &oss, const Callback &c);

                /** \return a string describing the callback (eg: client id) */
                virtual void            ToString(std::ostream &oss) const       {}

                /** \return the tcp handler. */
                TcpConnection   *GetTcpConnection()         {return _tcpConnection;}
                /** Set the connection used by the callback. */
                virtual void    SetupConnection(TcpConnection *tcpConnection);

                /** Call the given callback, can be redefined, to for exemple manage a ping method and check if clients are responding. */
                virtual void    CallCallback(Method callback, const Packet &p);

                /** Update the flow control, check acked packets and send states using the flow control class. */
                virtual void    Update(float elapsedTime);

                /** \return the tcp layer taking care of the protocol. */
                Layer           &TcpLayer()             {return _tcpLayer;}

            protected:
                TcpConnection   *_tcpConnection;            ///< handler for the tcp connection.
                Layer           _tcpLayer;                  ///< manage the tcp protocol application layer.
        };
    }
}

#endif
