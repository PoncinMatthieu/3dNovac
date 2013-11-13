
#include "Callback.h"
#include "TcpConnection.h"
#include "UdpConnection.h"

using namespace Nc;
using namespace Nc::Net;

Callback::Callback(TcpConnection *tcpConnection, UdpConnection *udpConnection)
    : _tcpConnection(tcpConnection), _udpConnection(udpConnection), _sendStates(false), _lastUpdateStatesTime(0)
{
    if (_tcpConnection != NULL)
    {
        _tcpConnection->SetCallbackInstance(this);
        _tcpConnection->ClearCallbacks();
    }
}

Callback::~Callback()
{
}

namespace Nc
{
    namespace Net
    {
        std::ostream& operator << (std::ostream &oss, const Callback& c)
        {
            c.ToString(oss);
            return oss;
        }
    }
}

void    Callback::SetupConnections(TcpConnection *tcpConnection, UdpConnection *udpConnection)
{
    _tcpConnection = tcpConnection;
    _udpConnection = udpConnection;

    if (_tcpConnection != NULL)
    {
        _tcpConnection->SetCallbackInstance(this);
        _tcpConnection->ClearCallbacks();
    }
}

void    Callback::CallCallback(Method callback, const Packet &p)
{
    CALLSTACK_INFO();

    (this->*callback)(p);
}

void    Callback::Update(float elapsedTime)
{
    CALLSTACK_INFO();

    if (_tcpConnection != NULL && !_tcpConnection->IsResponding())
    {
        LOG_ERROR << "Callback::Update: The client is not responding." << std::endl;
        _tcpConnection->Disconnect();
    }

    // no need to update the flow control if the udp is not set.
    if (_udpConnection == NULL)
        return;

    // update the flow control class.
    _flowControl.Update(elapsedTime, _udpLayer.RTT());

    // stop here if we don't want to send any states
    if (!_sendStates)
        return;

    // check acked packets
    CheckLostPackets(_udpLayer.LostPackets());

    // send the game state on the client at the good moment.
    _lastUpdateStatesTime += elapsedTime;
    float sendRate = 1.f / (float)_flowControl.SendRate();
    const float epsilon = 0.001f;
    if (_lastUpdateStatesTime >= sendRate - epsilon)
    {
        SendStates();
        _lastUpdateStatesTime = 0;

        // now, to get the good RTT and manage the flow control properly, we need to send an ack if nothing has been sent
        if (!_udpConnection->HasPendingRequest())
            _udpConnection->PushAck(this);
    }

    #ifdef _DEBUG
        static float time = 0;
        if (time > 2)
        {
            std::string client = Utils::Convert::ToString(*this);
            if (!client.empty())
                LOG << client << " ";
            LOG << "RTT: " << _udpLayer.RTT() * 1000 << " Sent: " << _udpLayer.TotalSentPackets() << " Recv: " << _udpLayer.TotalReceivedPackets()
                << " Lost: " << _udpLayer.TotalLostPackets() << " (" << ((_udpLayer.TotalSentPackets() > 0) ? (float)_udpLayer.TotalLostPackets() / (float) _udpLayer.TotalSentPackets() * 100 : 0) << "%) "
                << " Sent bandwidth: " << _udpLayer.SentPacketsPerSecond() << "Pps " << _udpLayer.SentBandwidth() << "kbps" << std::endl;
            time = 0;
        }
        time += elapsedTime;
    #endif
}
