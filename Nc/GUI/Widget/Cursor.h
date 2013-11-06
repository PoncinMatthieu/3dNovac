
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

    File Created At:        06/11/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_CURSOR_H_
#define NC_GUI_WIDGET_CURSOR_H_

#include "Image.h"

namespace Nc
{
    namespace GUI
    {
        /// Define a cursor
        /**
            The cursor automatically follow the position of the mouse and is aligned on the given center in the constructor.
            The cursor must be put on the scene on top of all other elements.
        */
        class Cursor : public Image
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Image, System::Object, Nc::GUI::Cursor);

            public:
                Cursor(Graphic::IWindow *window, Graphic::Sprite *sprite, const Math::Vector2i &center = Math::Vector2i(0,0));

            protected:
                /** Update the position of the cursor. */
                virtual void    Update(float elapsedTime);

            private:
                Graphic::IWindow    *_window;
                Math::Vector2i      _center;
        };
    }
}

#endif
