
#include "Callback.h"
#include "TcpConnection.h"

using namespace Nc;
using namespace Nc::Net;

Callback::Callback(TcpConnection *tcpConnection)
    : _tcpConnection(tcpConnection)
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

void    Callback::SetupConnection(TcpConnection *tcpConnection)
{
    _tcpConnection = tcpConnection;

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
}
