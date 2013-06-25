
#include <botan/tls_client.h>
#include <botan/tls_server.h>

#include <botan/rsa.h>
#include <botan/x509cert.h>

#include <botan/x509stor.h>

#include "TlsConnection.h"

using namespace Nc;
using namespace Nc::Net;

Botan::Private_Key           *TlsConnection::_serverKey = NULL;
Botan::X509_Certificate      *TlsConnection::_serverCert = NULL;
Botan::X509_Certificate      *TlsConnection::_caCert = NULL;

void    TlsConnection::LoadServerCert(const Nc::Utils::FileName &serverCert, const Nc::Utils::FileName &serverKey)
{
    Botan::AutoSeeded_RNG rng;
    _serverCert = new Botan::X509_Certificate(serverCert);
    _serverKey = Botan::PKCS8::load_key(serverKey, rng);
}

void    TlsConnection::LoadCA(const Nc::Utils::FileName &caCert)
{
    _caCert = new Botan::X509_Certificate(caCert);
}

void    TlsConnection::CleanupCerts()
{
    if (_serverKey != NULL)
        delete _serverKey;
    if (_serverCert != NULL)
        delete _serverCert;
    if (_caCert != NULL)
        delete _caCert;
}

bool TlsConnection::TlsPolicy::check_cert(const std::vector<Botan::X509_Certificate>& certs) const
{
    if (_caCert == NULL)
        throw Utils::Exception("Can't accept connection, CA isn't loaded!");

    Botan::X509_Store cert_store;
    cert_store.add_cert(*_caCert, /*trusted=*/true);

    for(size_t i = 0; i != certs.size(); ++i)
    {
        /*
        LOG_DEBUG << "Certif: " << std::endl;
        LOG_DEBUG << certs[i].to_string();
        */
        Botan::X509_Code code = cert_store.validate_cert(certs[i]);
        if(code == Botan::VERIFIED)
            LOG_DEBUG << "Cert validated" << std::endl;
        else
        {
            LOG_DEBUG << "Cert did not validate, code = " << code << std::endl;
            return false;
        }
    }
    return true;
}


TlsConnection::TlsConnection()
    : TcpConnection(), _tls(NULL)
{
}

TlsConnection::TlsConnection(const Nc::Network::Ip &ip, unsigned short port)
    : TcpConnection(ip, port), _tls(NULL)
{
}

TlsConnection::~TlsConnection()
{
    if (_tls != NULL)
    {
        delete _tls;
    }
}

void    TlsConnection::Connect()
{
    CALLSTACK_INFO();
    TcpConnection::Connect();
    _tls = new Botan::TLS_Client(   std::tr1::bind(&Nc::Network::SocketTcp::Read, std::tr1::ref(_socket), Botan::_1, Botan::_2),
                                    std::tr1::bind(&Nc::Network::SocketTcp::Write, std::tr1::ref(_socket), Botan::_1, Botan::_2),
                                    _tlsPolicy, _rng);

    LOG_DEBUG << "Handshake established..." << std::endl;
}

void    TlsConnection::SendRequests()
{
    CALLSTACK_INFO();
    if (_tls == NULL)
        throw Utils::Exception("Tls connection not initialized !");

    while (!_pendingRequests.empty())
    {
        Nc::Math::Array<unsigned char> &r = _pendingRequests.front();
        if (_sendingState)
            _tls->write(r.data, r.Size());
        _pendingRequests.pop_front();
    }
    _tls->Flush();
}

void    TlsConnection::ReceiveRequests()
{
    CALLSTACK_INFO();
    if (_tls == NULL)
        throw Utils::Exception("Tls connection not initialized !");

    int r = _tls->read(_buffer, BUFF_SIZE);
    if (r <= 0)
    {
        _socket.Close();
    }
    else
    {
        unsigned int pos = 0;
        while (pos < r)
            pos += ManageReceivedPacket(_buffer + pos, r - pos);
    }
}

bool    TlsConnection::Accepted()
{
    CALLSTACK_INFO();

    if (_serverCert == NULL || _serverKey == NULL)
        throw Utils::Exception("Can't accept connection, server certif isn't loaded!");

    try
    {
        _tls = new Botan::TLS_Server(   std::tr1::bind(&Nc::Network::SocketTcp::Read, std::tr1::ref(_socket), Botan::_1, Botan::_2),
                                        std::tr1::bind(&Nc::Network::SocketTcp::Write, std::tr1::ref(_socket), Botan::_1, Botan::_2),
                                        _tlsPolicy, _rng, *_serverCert, *_serverKey);
    }
    catch (std::exception &e)
    {
        LOG_ERROR << "Handshake failed with error: " << e.what() << ". closing the socket..." << std::endl;
        _socket.Close();
        return false;
    }

    LOG_DEBUG << "Handshake established." << std::endl;
    return true;
}
