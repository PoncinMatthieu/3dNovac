
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

#include <errno.h>
#include <Nc/Core/System/Config.h>
#include "UdpConnection.h"

using namespace Nc;
using namespace Nc::Net;

UdpConnection::UdpConnection()
    : Connection<Network::SocketUdp>()
{
    _senderPort = 0;
    _hasPendingRequests = false;
}

UdpConnection::UdpConnection(unsigned short startingBoundPort)
    : Connection<Network::SocketUdp>()
{
    _senderPort = 0;
    _hasPendingRequests = false;
    Bind(startingBoundPort);
}

UdpConnection::~UdpConnection()
{
    _socket.Close();
}

void    UdpConnection::Update(float elapsedTime)
{
    for (ListSender::iterator it = _clients.begin(); it != _clients.end(); ++it)
        if (it->instance->UdpLayer().Update(elapsedTime, it->pendingRequest))
            _hasPendingRequests = true;
}

void    UdpConnection::Bind(unsigned short startingBoundPort)
{
    _socket.Create();

    if (!_socket.IsValid())
    {
        LOG_ERROR << "UdpConnection::Bind: Failed to create the socket." << std::endl;
        return;
    }

    while (_socket.Bind(startingBoundPort) == false && startingBoundPort != 0)
    {
        ++startingBoundPort;
    }
    if (startingBoundPort != 0)
    {
        LOG_DEBUG << "UdpConnection::Bind: Bind on port " << startingBoundPort << std::endl;
        // set the socket non blocking
        _socket.SetBlocking(false);
    }
    else
    {
        LOG_ERROR << "UdpConnection::Bind: Failed to find an open port." << std::endl;
        _socket.Close();
    }
}

void    UdpConnection::InitSelect(Nc::Network::Select &select)
{
    if (_socket.IsValid())
    {
        select.SetForReading(_socket);
        if (HasPendingRequest())
            select.SetForWriting(_socket);
    }
}

void    UdpConnection::CheckIO(Nc::Network::Select &select)
{
    // if we have something to write ... write
    if (select.IsReadyForWriting(_socket))
        SendRequests();
    // if we received something on the socket ... read
    if (select.IsReadyForReading(_socket))
        ReceiveRequests();
}

unsigned int    UdpConnection::ManageReceivedPacket(unsigned char *buffer, unsigned short readSize, const Nc::Network::Ip &ip, unsigned short port)
{
    // search for the sender
    Sender *sender = NULL;
    for (ListSender::iterator it = _clients.begin(); sender == NULL && it != _clients.end(); ++it)
        if(it->ip == ip && it->port == port)
            sender = &(*it);
    if (sender != NULL)
    {
        unsigned int headerSize = sender->instance->UdpLayer().HeaderSize();
        if (readSize < headerSize) // if size inferior to the header size
        {
            Error(UNKNOWN_PACKET);
            return readSize;
        }

        //unsigned short  code = 0;
        unsigned short  sequence = 0;
        if (sender->instance->UdpLayer().ParseHeader(buffer, readSize/*, code */, sequence))
        {
            unsigned int pos = headerSize;
            while (pos <= (readSize - sizeof(ReliableLayer::DataHeader)))
            {
                ReliableLayer::DataHeader   *dataHeader = (ReliableLayer::DataHeader*)(buffer + pos);
                unsigned char               *packet = buffer + pos + sizeof(ReliableLayer::DataHeader);

                pos += sizeof(ReliableLayer::DataHeader);
                pos += dataHeader->size;
                if (pos > readSize)
                    Error(CORRUPTED_PACKET);
                else
                {
                    // manage the packet
                    MapCallbackMethod::iterator it = sender->methods.find(dataHeader->code);
                    if (it != sender->methods.end())
                    {
                        if (!it->second.mostRecentOnly || it->second.currentSequence == -1 || PacketQueue::SequenceMoreRecent(sequence, it->second.currentSequence))
                        {
                            it->second.currentSequence = sequence;
                            sender->instance->CallCallback(it->second.method, Packet(packet, dataHeader->size));
                        }
                        else
                            LOG_DEBUG << "Droping packet." << std::endl;
                    }
                    else
                        Error(UNKNOWN_PACKET);
                }
            }
        }
    }
    else
        Error(UNKNOWN_SENDER);
    return readSize;
}

void    UdpConnection::AddNewCallback(unsigned short code, bool mostRecentOnly, Callback *instance, Callback::Method method)
{
    // init the udp layer before we add the callback
    instance->UdpLayer().Init();

    bool found = false;
    for (ListSender::iterator it = _clients.begin(); !found && it != _clients.end(); ++it)
    {
        if (it->instance == instance)
        {
            found = true;
            CallbackMethod &c = it->methods[code];
            c.method = method;
            c.currentSequence = -1;
            c.mostRecentOnly = mostRecentOnly;
        }
    }

    // if not found insert a new callback instance
    if (!found)
        LOG_ERROR << "UdpConnection::AddNewCallback: Failed to retreive the callback instance" << std::endl;
}

int     UdpConnection::PushRequest(unsigned char *packet, unsigned short packetSize, Callback *senderInstance)
{
    // retreive the sender
    Sender *sender = NULL;
    for (ListSender::iterator it = _clients.begin(); sender == NULL && it != _clients.end(); ++it)
        if (it->instance == senderInstance)
            sender = &(*it);
    if (sender == NULL || !sender->ip.IsValid())
        Error(UNKNOWN_SENDER);
    else
    {
        unsigned int headerSize = sender->instance->UdpLayer().HeaderSize();

        // check the size of the packet, the size shouldn't be longer than BUFF_SIZE to be received full.
        // in Udp, packet too long are simply discarded, so the emitter has to be carefull not to sent too much data.
        if (packetSize + headerSize > BUFF_SIZE)
            System::Config::Warning("UdpConnection::PushRequest", "Sending a request too big for the receiving buffer.");
        else
        {
            sender->pendingRequest.push_back(Nc::Math::Array<unsigned char>());
            sender->pendingRequest.back().InitSize(packetSize + headerSize);
            unsigned short packetIdentifier = sender->instance->UdpLayer().BuildHeader(sender->pendingRequest.back().data, packetSize);
            if (packetSize != 0 && packet != NULL)
                memcpy(sender->pendingRequest.back().data + headerSize, packet, packetSize);
            _hasPendingRequests = true;
            return packetIdentifier;
        }
    }
    return -1;
}

int     UdpConnection::PushRequest(unsigned char *packet, unsigned short code, unsigned short packetSize, Callback *senderInstance)
{
    // retreive the sender
    Sender *sender = NULL;
    for (ListSender::iterator it = _clients.begin(); sender == NULL && it != _clients.end(); ++it)
        if (it->instance == senderInstance)
            sender = &(*it);
    if (sender == NULL || !sender->ip.IsValid())
        Error(UNKNOWN_SENDER);
    else
    {
        unsigned int headerSize = sender->instance->UdpLayer().HeaderSize();

        // check the size of the packet, the size shouldn't be longer than BUFF_SIZE to be received full.
        // in Udp, packet too long are simply discarded, so the emitter has to be carefull not to sent too much data.
        if (packetSize + headerSize > BUFF_SIZE)
            System::Config::Warning("UdpConnection::PushRequest", "Sending a request too big for the receiving buffer.");
        else
        {
            sender->pendingRequest.push_back(Nc::Math::Array<unsigned char>());
            sender->pendingRequest.back().InitSize(packetSize + headerSize + sizeof(ReliableLayer::DataHeader));
            unsigned short packetIdentifier = sender->instance->UdpLayer().BuildHeader(sender->pendingRequest.back().data, packetSize);
            ReliableLayer::DataHeader *dataHeader = (ReliableLayer::DataHeader*)(sender->pendingRequest.back().data + headerSize);
            dataHeader->code = code;
            dataHeader->size = packetSize;
            if (packetSize != 0 && packet != NULL)
                memcpy(sender->pendingRequest.back().data + headerSize + sizeof(ReliableLayer::DataHeader), packet, packetSize);
            _hasPendingRequests = true;
            return packetIdentifier;
        }
    }
    return -1;
}

int     UdpConnection::PushAck(Callback *sender)
{
    return PushRequest(NULL, 0, sender);
}

void    UdpConnection::SendRequests()
{
    for (ListSender::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        while (!it->pendingRequest.empty())
        {
            if (_sendingState)
            {
                if (!_socket.Write(it->pendingRequest.front(), it->ip, it->port))
                    LOG_ERROR << "UdpConnection::SendRequests: Write failed on " << it->ip.ToString() << ":" << it->port << std::endl;
            }
            it->pendingRequest.pop_front();
        }
        _hasPendingRequests = false;
    }
}

void    UdpConnection::ReceiveRequests()
{
    Nc::Network::Ip senderIp;
    unsigned short  senderPort = 0;

    // with udp, a write on the pipe is equal to a read on the other side.
    // (insteed of tcp where packets can be merged and many write can result in one read.)
    // so we need to read until we read 0 bytes
    int r;
    while ((r = _socket.Read(_buffer, BUFF_SIZE, senderIp, senderPort)) > 0)
        ManageReceivedPacket(_buffer, r, senderIp, senderPort);
}

void    UdpConnection::AddCallbackInstance(Callback *instance, const Nc::Network::Ip &ip, unsigned short port)
{
    _clients.push_back(Sender());
    _clients.back().instance = instance;
    _clients.back().ip = ip;
    _clients.back().port = port;
}

void    UdpConnection::RemoveCallbackInstance(Callback *instance)
{
    for (ListSender::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->instance == instance)
        {
            _clients.erase(it);
            return;
        }
    }
}

void    UdpConnection::Connect(Callback *sender)
{
    PushRequest(NULL/*, UdpGame::Client::ACK*/, 0, sender);
}

void    UdpConnection::Disconnect()
{
    _socket.Close();
}
