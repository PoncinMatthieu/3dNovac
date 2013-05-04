
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

    File Created At:        03/08/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "InputListener.h"
#include "Input.h"

using namespace Nc;
using namespace Nc::System;

Input::Input()
{
}

Input::~Input()
{
    while (!_listeners.empty())
    {
        (*_listeners.begin())->RemoveInput(this);
    }
}

void    Input::PushEvent(const Event &e)
{
    System::Locker l(&_mutex);
    for (ListenerList::iterator it = _listeners.begin(); it != _listeners.end(); ++it)
    {
        System::Locker l(&(*it)->GetMutex());
        (*it)->GetEventQueue().push(e); // push l'event dans la queue
    }
}



