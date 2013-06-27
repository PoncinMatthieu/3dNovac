
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

#ifndef NC_NET_RELIABLELAYER_H_
#define NC_NET_RELIABLELAYER_H_

#include <assert.h>
#include <list>
#include <vector>
#include <Nc/Core/Math/Math.h>
#include "Define.h"
#include "Layer.h"

namespace Nc
{
    namespace Net
    {
        struct PacketData
        {
            unsigned short sequence;        // packet sequence number
            float time;					    // time offset since packet was sent or received (depending on context)
            int size;						// packet size in bytes
        };

        struct PacketQueue : public std::list<PacketData>
        {
            /**
                The method manage the wrap around of the sequence number. (When the sequence number reach the maximum number and then go back to 0)
                \return true if the sequence \p s1 is more recent than \p s2.
             */
            static bool SequenceMoreRecent(unsigned short s1, unsigned short s2);

            /** \return true id the \p sequence exist in the list. */
            bool Exists(unsigned int sequence);

            /** insert the given \p packet into the list in sort of keeping the list sorted. */
            void InsertSorted(const PacketData &p);
        };

        /// Provide a realiable layer on top of udp, to provide reliability and flow control.
        class LIB_NC_NET ReliableLayer
        {
            public:
                struct DataHeader
                {
                    unsigned char       code;           ///< command id, to identify the data type.
                    unsigned short      size;           ///< number of packets inside the data
                };

            private:
                struct ReliableHeader
                {
                    //unsigned short      code;           ///< command id, to identify the command.
                    unsigned short      sequence;       ///< sequence number to identify packets.
                    unsigned short      ack;            ///< to acknoledge the reception of a packets.
                    unsigned int        ackBitfield;    ///< store every ack (32) received before ack. (using redundancy to defeat packet loss)
                };

            public:
                ReliableLayer();
                ~ReliableLayer();

                /** Initialize the reliable layer. It must be initialized in case of reconnection. */
                void            Init();

                /**
                    Update the queues and the stats.
                    If the pending request is empty and we need to send an ack, push a new packet into the pending request.
                    \return true, if we added a new packet into the pending requests.
                */
                bool            Update(float elapsedTime, std::list<Nc::Math::Array<unsigned char> > &pendingRequest);

                /** \return the size of the header. */
                unsigned int    HeaderSize()                        {return sizeof(ReliableHeader);}

                /**
                    Build a new header with the given code command and size.
                    \return the sequence number of the packet.
                */
                unsigned short  BuildHeader(unsigned char *packet, unsigned short packetSize);

                /**
                    Parse the given packet to extract the header.
                    \return true if the packet should be managed, fill in the code command and sequence number of the packet.
                */
                bool            ParseHeader(unsigned char *packet, unsigned short packetSize, unsigned short &sequence);

                static float            RTTMax()                            {return _rttMaximum;}
                /** \return the Round Trip Time of the client. */
                float                   RTT() const                         {return _rtt;}
                /** \return the total number of packets sent. */
                unsigned int            TotalSentPackets() const            {return _sentPackets;}
                /** \return the total number of packets received. */
                unsigned int            TotalReceivedPackets() const        {return _recvPackets;}
                /** \return the total number of packets lost. */
                unsigned int            TotalLostPackets() const            {return _lostPackets;}
                /** \return the number of packets sent over the last second. */
                unsigned int            SentPacketsPerSecond() const        {return _sentPacketsPerSecond;}
                /** \return the bandwidth sent over the last second. */
                float                   SentBandwidth() const               {return _sentBandwidth;}

                /** \return the queue of acked packet from last set of packet received. */
                //const PacketQueue       &AckedPackets() const               {return _ackedQueue;}
                /** \return the packet lost since last update. */
                const PacketQueue       &LostPackets() const                {return _lostQueue;}

            private:
                /** \return the ack bitfield to put into the header. It represent the ack for the last 32 packet before ack. */
                unsigned int        GenerateAckBits();
                /** \return the index of the bit for the given sequence into the bitfield. */
                int                 IndexBitForSequence(unsigned short sequence, unsigned short ack);
                /** Update the queues. */
                void                UpdateQueues(float elapsedTime);
                /** Update the stats. */
                void                UpdateStats();

            private:
                unsigned short      _localSequence;         ///< local sequence number for most recently sent packet
                unsigned short      _remoteSequence;        ///< remote sequence number for most recently received packet

                unsigned int        _sentPackets;           ///< total number of packets sent
                unsigned int        _recvPackets;		    ///< total number of packets received
                unsigned int        _lostPackets;		    ///< total number of packets lost
                unsigned int        _ackedPackets;		    ///< total number of packets acked

                float               _sentBandwidth;		    ///< approximate sent bandwidth over the last second
                float               _rtt;				    ///< estimated round trip time
                unsigned int        _sentPacketsPerSecond;  ///< number of packet sent over the last second.

                PacketQueue         _sentQueue;	            ///< sent packets used to calculate sent bandwidth (kept until rtt_maximum)
                PacketQueue         _receivedQueue;         ///< received packets for determining acks to send (kept up to most recent recv sequence - 32)
                PacketQueue         _pendingAckQueue;       ///< sent packets which have not been acked yet (kept until rtt_maximum * 2 )
                //PacketQueue         _ackedQueue;          ///< acked packets from last set of packet received. cleared at each update!
                PacketQueue         _lostQueue;             ///< store every lost packet since last update.

                static float        _rttMaximum;	        ///< maximum expected round trip time (hard coded to one second for the moment)
        };
    }
}

#endif
