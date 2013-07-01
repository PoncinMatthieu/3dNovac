
#include "TcpConnection.h"

using namespace Nc;
using namespace Nc::Net;

TcpConnection::TcpConnection()
    : Connection<Nc::Network::SocketTcp>(), _port(0), _callbackInstance(NULL)
{
}

TcpConnection::TcpConnection(const Nc::Network::Ip &ip, unsigned short port)
    : Connection<Nc::Network::SocketTcp>(), _port(port), _ip(ip), _callbackInstance(NULL)
{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::Disconnect()
{
    if (Connected())
    {
        CALLSTACK_INFO_ARG("Close connection from " + _ip.ToString());
        _socket.Close();
    }
}

void TcpConnection::Connect()
{
    CALLSTACK_INFO_ARG("Attempting to connect on " + _ip.ToString());

    if (!_ip.IsValid())
        LOG_ERROR << "TcpConnection::Connect: Could not resolve host, ip not valid." << std::endl;
    else
    {
        // close the previous connection
        Disconnect();

        // create the socket and connection to the ip/port specified
        _socket.Create();
        if (!_socket.Connect(_ip, _port) || !Connected())
        {
            _socket.Close();
            LOG_ERROR << "TcpConnection::Connect: Could not connect on the server." << std::endl;
        }
        else
        {
            //LOG_DEBUG << "Connection to the server succeed!" << std::endl;
        }
    }
}

void    TcpConnection::Listen(Nc::UInt16 port)
{
    CALLSTACK_INFO();

    _socket.Create();
    _socket.Listen(port);
}

bool    TcpConnection::Accept(TcpConnection &newConnection)
{
    CALLSTACK_INFO();

    Nc::Network::Ip newIp;
    if (!_socket.Accept((Nc::Network::SocketTcp&)newConnection.Socket(), newIp) || !newConnection.Connected())
    {
        LOG_ERROR << "TcpConnection::Accept: accept failed." << std::endl;
        return false;
    }

    newConnection.Ip(newIp);
    return newConnection.Accepted();
}

void    TcpConnection::InitSelect(Nc::Network::Select &select)
{
    CALLSTACK_INFO();
    if (_socket.IsValid())
    {
        select.SetForReading(_socket);
        if (HasPendingRequests())
        {
            select.SetForWriting(_socket);
        }
    }
}

void    TcpConnection::CheckIO(Nc::Network::Select &select)
{
    CALLSTACK_INFO();
    // if we have something to write ... write
    if (select.IsReadyForWriting(_socket))
        SendRequests();
    // if we received something on the socket ... read
    if (select.IsReadyForReading(_socket))
        ReceiveRequests();
}

unsigned int    TcpConnection::ManageReceivedPacket(unsigned char *buffer, unsigned int readSize)
{
    CALLSTACK_INFO();
    unsigned int headerSize = _callbackInstance->TcpLayer().HeaderSize();
    if (readSize < headerSize) // if size inferior to the header size
    {
        Error(UNKNOWN_PACKET);
        return readSize;
    }

    unsigned short  packetSize = 0;
    unsigned short  code = _callbackInstance->TcpLayer().ParseHeader(buffer, packetSize);
    unsigned char   *packet = buffer + headerSize;

    // manage the packet. If ManagePacket return false, the packet was not full
    if ((readSize - headerSize) < packetSize)
    {
        Error(UNKNOWN_PACKET);
        return readSize;
    }

    MapCallback::iterator it = _callbacks.find(code);
    if (it != _callbacks.end())
    {
        try
        {
            _callbackInstance->CallCallback(it->second, Packet(packet, packetSize));
            _timer.Reset(); // reset the timer
        }
        catch (std::exception &e)
        {
            LOG_ERROR << e.what() << std::endl;
            Error(CORRUPTED_PACKET);
        }
    }
    else
        Error(UNKNOWN_PACKET);
    return packetSize + headerSize;
}

void    TcpConnection::AddNewCallback(unsigned short code, Callback::Method method)
{
    _callbacks[code] = method;
}

void    TcpConnection::PushRequest(unsigned char *packet, unsigned short code, unsigned short packetSize)
{
    CALLSTACK_INFO();
    unsigned int headerSize = _callbackInstance->TcpLayer().HeaderSize();
    _pendingRequests.push_back(Nc::Math::Array<unsigned char>());
    _pendingRequests.back().InitSize(packetSize + headerSize);
    _callbackInstance->TcpLayer().BuildHeader(_pendingRequests.back().data, code, packetSize);
    if (packetSize != 0 && packet != NULL)
        memcpy(_pendingRequests.back().data + headerSize, packet, packetSize);
}

void    TcpConnection::SendRequests()
{
    CALLSTACK_INFO();
    while (!_pendingRequests.empty())
    {
        Nc::Math::Array<unsigned char> &r = _pendingRequests.front();
        if (_sendingState)
        {
            _socket.WriteDatagram(r);
        }
        _pendingRequests.pop_front();
    }
}

void    TcpConnection::ReceiveRequests()
{
    CALLSTACK_INFO();
    int r = _socket.Read(_buffer, BUFF_SIZE);
    if (r > 0)
    {
        unsigned int pos = 0;
        while ((int)pos < r)
            pos += ManageReceivedPacket(_buffer + pos, r - pos);
    }
}

