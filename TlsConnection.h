
#ifndef NC_NET_TLSCONNECTION_H_
#define NC_NET_TLSCONNECTION_H_

#include <botan/botan.h>
#include <botan/tls_connection.h>
#include <botan/tls_policy.h>
#include "TcpConnection.h"

namespace Nc
{
    namespace Net
    {
        /// Allow to manage a Transport Layer Security over a Tcp socket connection.
        /**
            This class use Botan library, you must initialize the library first by creating an object LibraryInitializer.
        */
        class TlsConnection : public TcpConnection
        {
            private:
                struct TlsPolicy : public Botan::TLS_Policy
                {
                    bool check_cert(const std::vector<Botan::X509_Certificate>& certs) const;
                };

            public:
                TlsConnection();
                TlsConnection(const Network::Ip &ip, unsigned short port);
                ~TlsConnection();

                static void     LoadServerCert(const Utils::FileName &serverCert, const Utils::FileName &serverKey);
                static void     LoadCA(const Utils::FileName &caCert);
                static void     CleanupCerts();

                /** Attempt to connect to the GameServer and send a CONNECTION packet. */
                virtual void    Connect();

            private:
                /** Send every pending requests. */
                virtual void    SendRequests();
                /** Receive every requests. */
                virtual void    ReceiveRequests();
                /**
                    Called once a new connection is accepted succesfully (new client connection).
                    \return true if the connection is validated.
                */
                virtual bool    Accepted();

            private:
                Botan::AutoSeeded_RNG   _rng;
                Botan::TLS_Connection   *_tls;

                TlsPolicy               _tlsPolicy;

                static Botan::Private_Key           *_serverKey;
                static Botan::X509_Certificate      *_serverCert;
                static Botan::X509_Certificate      *_caCert;
        };
    }
}

#endif
