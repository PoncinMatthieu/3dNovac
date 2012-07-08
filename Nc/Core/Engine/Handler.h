
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

namespace Nc
{
    namespace Engine
    {
        /// Used to simplify the process of sending events to an Engine
        /**
            Inherite of this class to use it, and call the methods 'HandlerEngineName' and 'HandlerId' to setup an event.
            Then you can send an event to the predefined engine and with the predefined command id by calling the method 'SendEvent'.
        */
        class LCORE Handler
        {
            public:
                Handler();
                Handler(const std::string &engineName, unsigned int id);

                /** Set the name of the engine which will recieve the events */
                inline void                 HandlerEngineName(const std::string &name)      {_engineName = name;}
                /** \return the EngineName which will recieve the events */
                inline const std::string    &HandlerEngineName() const      {return _engineName;}

                /** Set the Id of the Cmd to send */
                inline void                 HandlerId(unsigned int id)                      {_idCmd = id;}
                /** \return the Id of the Cmd to send */
                inline unsigned int         HandlerId() const               {return _idCmd;}

            protected:
                /** Send to the GameManager an event, the GameManager will redispatch the event */
                template<typename T>
                void SendEvent(T &arg)                                      {Manager::PushEvent(_engineName, _idCmd, arg);}

                std::string     _engineName;    ///< The name of the engine to send the event
                unsigned int    _idCmd;         ///< The id of the cmd to send
        };
    }
}

#endif
