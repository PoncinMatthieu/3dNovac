
#include "ClientState.h"
#include "ReliableLayer.h"

using namespace Nc;
using namespace Nc::Net;

ClientState::State::State(bool initialState)
	: changed(initialState), lost(false), time(0)
{
}

bool    ClientState::State::Acked() const
{
	const float epsilon = 0.001f;
	return (!changed && !lost && time > ReliableLayer::RTTMax());
}

void    ClientState::State::AckLossSequence(unsigned short seq)
{
    if (!lost)
    {
        lost = true;
        for (SequenceList::iterator it = sequences.begin(); it != sequences.end(); ++it)
        {
            if (it->first == seq)
                it->second = true;
            if (it->second == false)
                lost = false;
        }
    }
}

void    ClientState::State::Update(unsigned short packetIdentifier)
{
    if (NeedUpdate())
    {
        if (Changed())
            ResetSequences();
        Reset();
        PushSequence(packetIdentifier);
    }
}

void    ClientState::State::UpdateTime(float elapsedTime)
{
    time += elapsedTime;
}


ClientState::ClientState()
    : _initialState(true)
{
}

ClientState::State	&ClientState::GetState(Callback *cl)
{
    MapState::iterator it = _clientStates.find(cl);
    if (it != _clientStates.end())
        return it->second;
    else
        return _clientStates.insert(it, MapState::value_type(cl, _initialState))->second;
}

void	ClientState::SetStateChanged()
{
	for (MapState::iterator it = _clientStates.begin(); it != _clientStates.end(); ++it)
		it->second.changed = true;
}

bool	ClientState::StateAcked()
{
    bool st = true;
    for (MapState::iterator it = _clientStates.begin(); it != _clientStates.end(); ++it)
        if (!it->second.Acked())
            st = false;
    return st;
}

