
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        28/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_ENUM_H_
#define NC_GUI_ENUM_H_

namespace Nc
{
    namespace GUI
    {
        /// enum to define a relative position of a Widget
        enum Corner
        {
            Top,        ///< should be used to define the Y relative position of the widget
            Bottom,     ///< should be used to define the Y relative position of the widget
            Left,       ///< should be used to define the X relative position of the widget
            Right,      ///< should be used to define the X relative position of the widget
            Center      ///< can be used to define the X or Y relative position of the widget
        };

        /// used to define an object orientation
        enum Orientation
        {
            Horizontal,
            Vertical
        };
    }
}

#endif
