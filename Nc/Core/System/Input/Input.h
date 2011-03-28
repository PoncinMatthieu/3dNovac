
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
/*-----------------------------------------------------------------------------

    provide an interface for Inputs
    CheckEvent needs to be redefined,
    and all the events need to be push into the _eventQueue pointer

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
        class LCORE Input
        {
            public:
                Input() {_eventQueue = NULL; _mutexQueue = NULL; _eventQueueIsSet = false;}
                virtual ~Input() {}

                void SetEventQueue(EventQueue *eventQueue, Mutex *mutexQueue)
                {
                    _eventQueue = eventQueue;
                    _mutexQueue = mutexQueue;
                    _eventQueueIsSet = true;
                }
                virtual void CheckEvents() = 0;

            protected:
                void PushEvent(const Event &e)
                {
                    if (!_eventQueueIsSet || _eventQueue == NULL)
                        throw Utils::Exception("Input", "The event queue is null");
                    if (_mutexQueue)
                        _mutexQueue->Lock();
                    _eventQueue->push(e); // push l'event dans la queue
                    if (_mutexQueue)
                        _mutexQueue->Unlock();
                }

                bool    _eventQueueIsSet;

            private:
                EventQueue      *_eventQueue;
                Mutex           *_mutexQueue;
        };
    }
}

#endif
