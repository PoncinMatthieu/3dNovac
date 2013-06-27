
#ifndef NC_NET_CLIENTSTATE_H_
#define NC_NET_CLIENTSTATE_H_

#include "Callback.h"

namespace Nc
{
    namespace Net
    {
        /// Store client states and packet sequence sent to manage packet loss.
        class LIB_NC_NET ClientState
        {
            public:
                struct LIB_NC_NET State
                {
                    typedef std::list<std::pair<unsigned short, bool> >     SequenceList;

                    State(bool initialState = true);

                    bool    Changed() const                     {return changed;}
                    void    Reset()                             {changed = false; time = 0;}

                    bool    Acked() const;
                    bool    Lost() const                        {return lost;}
                    void    PushSequence(unsigned short seq)    {sequences.push_back(std::pair<unsigned short, bool>(seq, false)); lost = false;}
                    void    ResetSequences()                    {sequences.clear();}
                    int     SizeSequenceList() const            {return sequences.size();}

                    void    AckLossSequence(unsigned short seq);

                    bool    NeedUpdate() const                  {return (changed || lost);}

                    void    Update(unsigned short packetIdentifier);

                    void    UpdateTime(float elapsedTime);

                    bool                changed;        ///< true if the path changed.
                    SequenceList        sequences;      ///< store the packet sequences numbers sent before last state changed.
                    bool                lost;           ///< true if the state has been lost.
                    float               time;           ///< time since last update.
                };
                typedef std::map<Callback*, State>      MapState;
                typedef std::list<State*>               ListState;

            public:
                ClientState();

                void            SetInitialState(bool s)                             {_initialState = s;}

                /** Remove the callback instance, important in case a callback is destroyed and a new new one is allocated using the same address. */
                void            RemoveState(Callback *cl)                           {_clientStates.erase(cl);}

                /** \return the state corresponding to the client. */
                State           &GetState(Callback *cl);

                /** Set every state changed to true. */
                void            SetStateChanged();

                /** \return true if the state is acked. */
                bool            StateAcked();

            private:
                bool            _initialState;
                MapState        _clientStates;      ///< store every client state.
        };
    }
}

#endif
