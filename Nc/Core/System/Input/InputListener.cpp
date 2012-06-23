
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

#include "InputListener.h"

using namespace std;
using namespace Nc::System;

InputListener::InputListener()
{
}

InputListener::~InputListener()
{
    for (ListInput::iterator it = _inputList.begin(); it != _inputList.end(); ++it)
    {
        (*it)->RemoveListener(this);
    }
}

void    InputListener::AddInput(Input *input)
{
    input->AddListener(this);
    _inputList.push_back(input);
}

void    InputListener::RemoveInput(Input *input)
{
    input->RemoveListener(this);
    _inputList.remove(input);
}

bool InputListener::PollEvent(Event &e)
{
// si il y a un evenement, on le depile et return true
    _mutexQueue.Lock();
    if (!_eventQueue.empty())
    {
        e = _eventQueue.front();
        _eventQueue.pop();
//        for (ListAction::iterator it = _inputActionList.begin(); it != _inputActionList.end(); ++it)
//            (*it)->CheckEvents(e);
        _mutexQueue.Unlock();
        return true;
    }
    _mutexQueue.Unlock();
    return false;
}
