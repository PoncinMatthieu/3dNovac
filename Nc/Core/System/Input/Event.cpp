
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
            return (key.Code == e.key.Code);
        case MouseButtonReleased:
        case MouseButtonPressed:
            return (mouseButton.Button == e.mouseButton.Button);
        case MouseMoved:
            return (mouseMove.X == e.mouseMove.X && mouseMove.Y == e.mouseMove.Y);
        case MouseWheelMoved:
            return (mouseWheel.Delta == e.mouseWheel.Delta);
        default:
            return (size.Width == e.size.Width && size.Height == e.size.Height);
    }
}
