
#ifndef NC_NET_DEFINE_H_
#define NC_NET_DEFINE_H_

#include <Nc/Core/Utils/Utils.h>

namespace Nc
{
    namespace Net
    {
        static const unsigned int BUFF_SIZE = 4096;

        enum ErrorCode
        {
            UNKNOWN_PACKET = 0,
            UNKNOWN_SENDER,
            CORRUPTED_PACKET
        };

        class TcpConnection;
        class TlsConnection;

        typedef std::list<TcpConnection*>       ListTcpConnection;
        typedef std::list<TlsConnection*>       ListTlsConnection;
    }
}

#endif
