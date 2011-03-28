
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
/*-----------------------------------------------------------------------------

    Implementation de la classe "System::InputManager"
    gere les inputs Window, et autre

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_INPUT_MANAGER_H_
#define NC_CORE_SYSTEM_INPUT_MANAGER_H_

#include <list>
#include <queue>
#include "../../Define.h"
#include "Input.h"
//#include "InputAction.h"

namespace Nc
{
    namespace System
    {
        class LCORE InputManager : public Utils::NonCopyable
        {
            private:
                typedef std::list<Input*>           ListInput;
//                typedef std::list<InputAction*>     ListAction;

            public:
                InputManager() {};
                virtual ~InputManager() {};

                inline void    AddInput(Input *input)           {input->SetEventQueue(&_eventQueue, &_mutexQueue); _inputList.push_back(input);}
//                inline void    AddAction(InputAction *action)   {_inputActionList.push_back(action);}

                bool    PollEvent(Event &e);

            private:
                ListInput       _inputList;
//                ListAction      _inputActionList;
                EventQueue      _eventQueue;
                Mutex           _mutexQueue;
        };
    }
}

#endif
