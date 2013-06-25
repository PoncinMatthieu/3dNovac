
#include "Layer.h"

using namespace Nc;
using namespace Nc::Net;

Layer::Layer()
{

}

Layer::~Layer()
{

}

unsigned short      Layer::BuildHeader(unsigned char *packet, unsigned short code, unsigned short packetSize)
{
    ((Header*)packet)->code = code;
    ((Header*)packet)->packetSize = packetSize;
    return 0;
}

unsigned short  Layer::ParseHeader(unsigned char *packet, unsigned short &packetSize)
{
    packetSize = ((Header*)packet)->packetSize;
    return ((Header*)packet)->code;
}
