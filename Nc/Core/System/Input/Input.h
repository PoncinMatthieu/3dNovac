
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

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

    File Created At:        06/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_INPUT_H_
#define NC_CORE_SYSTEM_INPUT_H_

#include "../../Define.h"
#include "../../System/API/API.h"
#include "Event.h"

namespace Nc
{
    namespace System
    {
        class InputListener;

        /// Provide an interface for Inputs
        /**
            Provide an interface for Inputs. An input as a list Listener.
            To add a Listener, use the method AddEventQueue.
            All the events need to be push with the method PushEvent.
            CheckEvent needs to be redefined.

            \sa
                - InputListener
        */
        class LCORE Input : public EventEmitter
        {
            protected:
                typedef std::list<InputListener*>               ListenerList;

            public:
                Input();
                virtual ~Input();

                /** Add the given listner to the listener list */
                inline void         AddListener(InputListener *listener)            {_mutex.Lock(); _listeners.push_back(listener); _mutex.Unlock();}
                /** Remove he given listner to the listener list */
                inline void         RemoveListener(InputListener *listener)         {_mutex.Lock(); _listeners.remove(listener); _mutex.Unlock();}

                /** To be redefine, called to check if there is new events */
                virtual void        CheckEvents() = 0;

                /** \return false if there are a listener */
                inline bool         ListenerListEmpty()                             {return _listeners.empty();}

            protected:
                /** Push a new event in the eventQueue pointer */
                void                PushEvent(const Event &e);

            private:
                ListenerList    _listeners;
                Mutex           _mutex;
        };
    }
}

#endif
