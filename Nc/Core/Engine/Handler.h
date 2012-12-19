
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The The 3dNovac Team

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

    File Created At:        10/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_HANDLER_H_
#define NC_CORE_ENGINE_HANDLER_H_

#include "../Define.h"
#include "../Utils/Utils.h"
#include "Manager.h"
#include "EventManager.h"

namespace Nc
{
    namespace Engine
    {
        /// Used to simplify the process of sending events to one or many EventManager.
        /**
            Store a list of EventManager to send events.
        */
        class LCORE Handler
        {
            private:
                typedef std::list<EventManager*>        ListEventManager;

            public:
                /** Add an engine from the Engine::Manager into the event manager list to sent events. */
                inline void         AddEngine(const std::string &name)      {AddEventManager(Manager::GetEngine(name));}
                /** Add the given event manager to the list to send events. */
                inline void         AddEventManager(EventManager *e)        {_eventManagers.push_back(e);}

                /** Send to the GameManager an event, the GameManager will redispatch the event */
                template<typename T>
                void    SendEvent(unsigned int idCmd, T &arg)
                {
                    for (ListEventManager::iterator it = _eventManagers.begin(); it != _eventManagers.end(); ++it)
                        (*it)->PushEvent(idCmd, arg);
                }

            private:
                ListEventManager    _eventManagers; ///< Instance on the event manager used to send events.
        };
    }
}

#endif
