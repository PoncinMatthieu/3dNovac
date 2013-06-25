
#ifndef NC_NET_CONNECTION_H_
#define NC_NET_CONNECTION_H_

#include <Nc/Core/Network/Select.h>
#include "Define.h"

namespace Nc
{
    namespace Net
    {
        /// Base class to manage a connection.
        template<class S>
        class Connection : Nc::Utils::NonCopyable
        {
            public:
                Connection()
                    : _closeSocketAtError(false), _sendingState(true), _connectionTimeout(60) {}
                ~Connection()                                       {_socket.Close();}

                /** Set a boolean to close the socket if we have a reception error. */
                void                CloseSocketAtError(bool state)  {_closeSocketAtError = state;}
                /** Set the sending state. */
                void                SendingState(bool state)        {_sendingState = state;}

                /** \return the socket. */
                S                   &Socket()                           {return _socket;}

                /** \return true if the connection is responding. (2 * _connectionTimeout) */
                bool                IsResponding() const                {return (_timer.ElapsedTime() < (_connectionTimeout * 2));}
                /** \return true if we should send a ping through the connection. (_connectionTimeout) */
                bool                ShouldPing() const                  {return (_timer.ElapsedTime() > (_connectionTimeout));}

            protected:
                /** Manage error messages. */
                void                Error(ErrorCode error);

            protected:
                bool                    _closeSocketAtError;            ///< if true, the socket will be close when the method Error is call.
                bool                    _sendingState;                  ///< if false, the handler will discard every packet to send.
                unsigned char           _buffer[BUFF_SIZE];		        ///< read buffer to read over the connection.
                Utils::Clock		_timer;			                ///< to test if the client is responding
                float			        _connectionTimeout;	            ///< time before we ping the client

                S                       _socket;                        ///< the socket tcp or udp.
        };

        template<class S>
        void    Connection<S>::Error(ErrorCode error)
        {
            switch (error)
            {
                case UNKNOWN_PACKET:		LOG_ERROR << "Connection::Error: Received unknown packet, discard..." << std::endl;   break;
                case UNKNOWN_SENDER:		LOG_ERROR << "Connection::Error: Sender not registered, discard..." << std::endl;     break;
                case CORRUPTED_PACKET:		LOG_ERROR << "Connection::Error: Packet corrupted, discard..." << std::endl;          break;
                default:                        LOG_ERROR << "Connection::Error: Unknown error." << std::endl; break;
            }
            if (_closeSocketAtError)
            {
                LOG_ERROR << "Connection::Error: Closing socket!" << std::endl;
                _socket.Close();
            }
        }
    }
}

#endif
