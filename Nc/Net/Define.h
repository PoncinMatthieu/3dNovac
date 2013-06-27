
#ifndef NC_NET_DEFINE_H_
#define NC_NET_DEFINE_H_

#include <Nc/Core/Utils/Utils.h>

// linking option (static or shared)
#ifdef SYSTEM_WINDOWS                                       // windows
    #ifdef NC_DYNAMIC_LINK                                      // dynamic
        #ifdef NC_EXPORT_NET
            #define LIB_NC_NET          __declspec(dllexport)
        #else
            #define LIB_NC_NET          __declspec(dllimport)
        #endif
    #else                                                   // static (do nothing)
        #define LIB_NC_NET
    #endif
#else                                                       // others os (do nothing)
    #define LIB_NC_NET                      ///< Linking option of Nc-Audio module
#endif

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
        class UdpConnection;
        class TlsConnection;

        typedef std::list<TcpConnection*>       ListTcpConnection;
        typedef std::list<TlsConnection*>       ListTlsConnection;
    }
}

#endif
