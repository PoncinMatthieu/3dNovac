
#ifndef NC_NET_PACKET_H_
#define NC_NET_PACKET_H_

#include "Define.h"

namespace Nc
{
    namespace Net
    {
        /// Encapsulate a received packet.
        class LIB_NC_NET Packet : public Utils::NonCopyable
        {
            public:
                Packet(unsigned char *data, unsigned int size)
                    : _data(data), _size(size)
                {
                }

                unsigned int Size() const
                {
                    return _size;
                }

                unsigned char   operator [] (unsigned int i) const
                {
                    if (i < _size)
                        return _data[i];
                    throw Utils::Exception("Packet::operator[" + Utils::Convert::ToString(i) + "]", "Overflow on the requested data!");
                }

                template<typename T>
                T       *Data(unsigned int offset = 0) const
                {
                    if (sizeof(T) > offset + _size)
                        throw Utils::Exception("Packet::Data", "Overflow on the requested data!");
                    return (T*)(_data + offset);
                }

            private:
                unsigned char   *_data;
                unsigned int    _size;
        };
    }
}

#endif

