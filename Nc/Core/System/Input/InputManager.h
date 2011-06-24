
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_INPUT_MANAGER_H_
#define NC_CORE_SYSTEM_INPUT_MANAGER_H_

#include <list>
#include <queue>
#include "../../Define.h"
#include "Input.h"

namespace Nc
{
    namespace System
    {
        /// Manage the inputs, like WindowInput
        class LCORE InputManager : public Utils::NonCopyable
        {
            private:
                typedef std::list<Input*>           ListInput;

            public:
                InputManager() {};
                virtual ~InputManager() {};

                /** Add the given input and set the eventQueue pointer of the input */
                inline void    AddInput(Input *input)           {input->SetEventQueue(&_eventQueue, &_mutexQueue); _inputList.push_back(input);}

                /** Fill the given event by the event wich was in the eventQueue */
                bool    PollEvent(Event &e);

            private:
                ListInput       _inputList;         ///< the list of inputs
                EventQueue      _eventQueue;        ///< the event queue witch store the event received by the inputs
                Mutex           _mutexQueue;        ///< the mutex used to protect the event queue
        };
    }
}

#endif
