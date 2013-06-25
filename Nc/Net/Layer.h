
#ifndef NC_NET_LAYER_H_
#define NC_NET_LAYER_H_

#include "Define.h"

namespace Nc
{
    namespace Net
    {
        /// Provide a Tcp layer. The base class only use a packet header containing a code command and it's size.
        /**
            \sa ReliableLayer, to be used with Udp
        */
        class Layer
        {
            private:
                struct Header
                {
                    unsigned short  code;
                    /**
                        We need to include the packetSize because this layer is used by Tcp
                        wich is a stream protocol where every packet can be bufferized and merged together.
                        For Udp we will use the Reliability layer, the packet size won't be usefull since the size will be equal to readSize - headerSize.
                    */
                    unsigned short  packetSize;
                };

            public:
                Layer();
                ~Layer();

                /** \return the size of the header. */
                unsigned int    HeaderSize() const              {return sizeof(Header);}

                /**
                    Build a new header with the given code command and packet size.
                    \return 0.
                */
                unsigned short  BuildHeader(unsigned char *packet, unsigned short code, unsigned short packetSize);

                /**
                    Parse the given packet to extract the header.
                    \return the code command of the packet and its size.
                */
                unsigned short  ParseHeader(unsigned char *packet, unsigned short &packetSize);
        };
    }
}

#endif
