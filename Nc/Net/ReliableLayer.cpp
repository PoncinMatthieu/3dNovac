
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

#include "ReliableLayer.h"

using namespace Nc;
using namespace Nc::Net;

bool PacketQueue::SequenceMoreRecent(unsigned short s1, unsigned short s2)
{
    return (s1 > s2) && (s1 - s2 <= 0xFFFF / 2) || (s2 > s1) && (s2 - s1 > 0xFFFF / 2);
}

bool PacketQueue::Exists(unsigned int sequence)
{
    for (iterator it = begin(); it != end(); ++it)
        if (it->sequence == sequence)
            return true;
    return false;
}

void PacketQueue::InsertSorted(const PacketData &p)
{
    if (empty())
        push_back(p);
    else
    {
        if (!SequenceMoreRecent(p.sequence, front().sequence))
            push_front(p);
        else if (SequenceMoreRecent(p.sequence, back().sequence))
            push_back(p);
        else
        {
            for (PacketQueue::iterator it = begin(); it != end(); it++)
            {
                assert(it->sequence != p.sequence);
                if (SequenceMoreRecent(it->sequence, p.sequence))
                {
                    insert(it, p);
                    break;
                }
            }
        }
    }
}

float ReliableLayer::_rttMaximum = 0.5f;

ReliableLayer::ReliableLayer()
{
    Init();
}

ReliableLayer::~ReliableLayer()
{

}

void    ReliableLayer::Init()
{
    _localSequence = 0;
    _remoteSequence = 0;
    _sentPackets = 0;
    _recvPackets = 0;
    _lostPackets = 0;
    _ackedPackets = 0;
    _sentBandwidth = 0.0f;
    _rtt = 0.0f;
    _sentPacketsPerSecond = 0;

    _sentQueue.clear();
    _receivedQueue.clear();
    _pendingAckQueue.clear();
    _lostQueue.clear();
}

unsigned short  ReliableLayer::BuildHeader(unsigned char *packet, unsigned short packetSize)
{
    ReliableHeader *header = (ReliableHeader*)packet;

    header->sequence = _localSequence;
    header->ack = _remoteSequence;
    header->ackBitfield = GenerateAckBits();

    Assert(!_sentQueue.Exists(_localSequence));
    Assert(!_pendingAckQueue.Exists(_localSequence));
    PacketData data;
    data.sequence = _localSequence;
    data.time = 0.0f;
    data.size = packetSize + HeaderSize();
    _sentQueue.push_back(data);
    _pendingAckQueue.push_back(data);
    _sentPackets++;
    _localSequence++;
    if (_localSequence > 0xFFFF)
        _localSequence = 0;
    return header->sequence;
}

bool  ReliableLayer::ParseHeader(unsigned char *packet, unsigned short packetSize, unsigned short &sequence)
{
    ReliableHeader *header = (ReliableHeader*)packet;

    _recvPackets++;
    if (_receivedQueue.Exists(header->sequence))
    {
        // if the packet has already been received, don't proceed.
        LOG << "Packet already received" << std::endl;
        return false;
    }
    else
    {
        PacketData data;
        data.sequence = header->sequence;
        data.time = 0.0f;
        data.size = packetSize + HeaderSize();
        _receivedQueue.push_back(data);
        if (PacketQueue::SequenceMoreRecent(header->sequence, _remoteSequence))
            _remoteSequence = header->sequence;
    }

    // check acks
    if (!_pendingAckQueue.empty())
    {
        PacketQueue::iterator it = _pendingAckQueue.begin();
        while (it != _pendingAckQueue.end())
        {
            bool acked = false;

            if (it->sequence == header->ack)
            {
                acked = true;
            }
            else if (!PacketQueue::SequenceMoreRecent(it->sequence, header->ack))
            {
                int bitIndex = IndexBitForSequence(it->sequence, header->ack);
                if (bitIndex <= 31)
                    acked = (header->ackBitfield >> bitIndex) & 1;
            }

            if (acked)
            {
                _rtt += (it->time - _rtt) * 0.1f; // smooth the rtt with a low pass filter.

                //_ackedQueue.InsertSorted(*it);
                ++_ackedPackets;
                it = _pendingAckQueue.erase(it);
            }
            else
                ++it;
        }
    }

    sequence = header->sequence;

    // if the packet was a ack (nothing else after the header), don't proceed.
    if (packetSize == HeaderSize())
        return false;
    return true;
}

unsigned int    ReliableLayer::GenerateAckBits()
{
    unsigned int ackBits = 0;
    for (PacketQueue::const_iterator it = _receivedQueue.begin(); it != _receivedQueue.end(); ++it)
    {
        if (it->sequence == _remoteSequence || PacketQueue::SequenceMoreRecent(it->sequence, _remoteSequence))
            break;
        int bitIndex = IndexBitForSequence(it->sequence, _remoteSequence);
        if (bitIndex <= 31)
            ackBits |= 1 << bitIndex;
    }
    return ackBits;
}

int     ReliableLayer::IndexBitForSequence(unsigned short sequence, unsigned short ack)
{
    // manage the wrap around of the sequence number
    if (sequence > ack)
    {
        Assert(ack < 33);
        Assert(0xFFFF >= sequence);
        return ack + (0xFFFF - sequence);
    }
    else
    {
        Assert(ack >= 1);
        Assert(sequence <= ack - 1);
        return ack - 1 - sequence;
    }
}

bool    ReliableLayer::Update(float elapsedTime, std::list<Nc::Math::Array<unsigned char> > &pendingRequest)
{
    //_ackedQueue.clear();
    _lostQueue.clear();
    UpdateQueues(elapsedTime);
    UpdateStats();
/*
    // if the pending request is empty and we need to send a ack for received packets
    // we add a packet into the pending requests
    if (pendingRequest.empty() && !_receivedQueue.empty())
    {
        // if we didn't sent a packet since 0.1, we send a packet to ack the received packets.
        // it will result in sending no more than 10 packets / seconds.
        if (_sentQueue.empty() || _sentQueue.back().time > 0.01f)
        {
            pendingRequest.push_back(Nc::Math::Array<char>(HeaderSize()));
            BuildHeader(pendingRequest.back().data, 0, 0);
            return true;
        }
    }
*/
    return false;
}

void    ReliableLayer::UpdateQueues(float elapsedTime)
{
    // update the time for every queue.
    for (PacketQueue::iterator it = _sentQueue.begin(); it != _sentQueue.end(); it++)
        it->time += elapsedTime;
    for (PacketQueue::iterator it = _receivedQueue.begin(); it != _receivedQueue.end(); it++)
        it->time += elapsedTime;
    for (PacketQueue::iterator it = _pendingAckQueue.begin(); it != _pendingAckQueue.end(); it++)
        it->time += elapsedTime;
    //for (PacketQueue::iterator it = _ackedQueue.begin(); it != _ackedQueue.end(); it++)
    //    it->time += elapsedTime;


    const float epsilon = 0.001f;
    // remove every sent packet after rtt max
    while (_sentQueue.size() && _sentQueue.front().time > 1.f + epsilon)
        _sentQueue.pop_front();

    // remove every sequence lesser than 33 sequence before the latest one.
    if (_receivedQueue.size())
    {
        const unsigned int latestSequence = _receivedQueue.back().sequence;
        const unsigned int minimumSequence = (latestSequence >= 34) ? (latestSequence - 34) : 0xFFFF - (34 - latestSequence);
        while (_receivedQueue.size() && !PacketQueue::SequenceMoreRecent(_receivedQueue.front().sequence, minimumSequence))
            _receivedQueue.pop_front();
    }

    // remove every acked packet after rtt max * 2
    //while (_ackedQueue.size() && _ackedQueue.front().time > _rttMaximum * 2 - epsilon)
    //    _ackedQueue.pop_front();

    // remove every pending acked packet after rtt max
    while (_pendingAckQueue.size() && _pendingAckQueue.front().time > _rttMaximum - epsilon)
    {
        _lostQueue.push_back(_pendingAckQueue.front());
        _pendingAckQueue.pop_front();
        _lostPackets++;
    }
}

void    ReliableLayer::UpdateStats()
{
    // we add 8 bytes for the size of the udp header.
    _sentPacketsPerSecond = 0;
    int sentBytesPerSecond = 0;
    for (PacketQueue::iterator it = _sentQueue.begin(); it != _sentQueue.end(); ++it)
    {
        sentBytesPerSecond += it->size + 8;
        ++_sentPacketsPerSecond;
    }

    sentBytesPerSecond /= 1.f;
    _sentBandwidth = sentBytesPerSecond * (8 / 1000.0f);
}
