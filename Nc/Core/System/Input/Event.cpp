
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

#include "Event.h"

using namespace Nc::System;

bool Event::operator == (const Event &e)
{
    if (type != e.type)
        return false;
    switch (type)
    {
        case KeyPressed:
        case KeyReleased:
            return (key.code == e.key.code);
        case MouseButtonReleased:
        case MouseButtonPressed:
            return (mouseButton.button == e.mouseButton.button);
        case MouseMoved:
            return (mouseMove.x == e.mouseMove.x && mouseMove.y == e.mouseMove.y);
        case MouseWheelMoved:
            return (mouseWheel.delta == e.mouseWheel.delta);
        default:
            return (size.width == e.size.width && size.height == e.size.height);
    }
}
