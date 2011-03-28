
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
    if (Type != e.Type)
        return false;
    switch (Type)
    {
        case KeyPressed:
        case KeyReleased:
            return (Key.Code == e.Key.Code);
        case MouseButtonReleased:
        case MouseButtonPressed:
            return (MouseButton.Button == e.MouseButton.Button);
        case MouseMoved:
            return (MouseMove.X == e.MouseMove.X && MouseMove.Y == e.MouseMove.Y);
        case MouseWheelMoved:
            return (MouseWheel.Delta == e.MouseWheel.Delta);
        default:
            return (Size.Width == e.Size.Width && Size.Height == e.Size.Height);
    }
}
