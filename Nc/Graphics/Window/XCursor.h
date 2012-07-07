
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_XCURSOR_H_
#define NC_GRAPHICS_WINDOW_XCURSOR_H_

#include <X11/Xlib.h>
#include <Nc/Core/Math/Math.h>
#include "ICursor.h"

namespace Nc
{
    namespace Graphic
    {
        class Window;

        /// Implementation of a ICursor for an X11 system.
        /**
            The cursor is actually loading the data with only 1 bit per pixel.

            To create a cursor you will need to have an XWindow but you shouldn't create directly a cursor, the best way is to use the method IWindow::NewCursor().

            \todo We have disabled cursor management when the window is created by using an existing window. Because cursor management cause freese bug with Qt.
            \todo Implement cursor loading with more than 1 bit per pixel (it's a problem on X11).
            \todo Implement a load from image method.
        */
        class Cursor : public ICursor
        {
            public:
                Cursor(Window *w);
                Cursor(const Cursor &c);
                virtual ~Cursor();

                /** \return a clone of the current cursor. */
                virtual ICursor     *Clone() const                  {return new Cursor(*this);}

                /** Enable the cursor. */
                virtual void Enable();

                /** Disable the cursor. */
                virtual void Disable();

                /** Load from an Xpm (for more detail show the detailed description of ICursor). */
                virtual void LoadFromXpm(const char *xpm[]);

                /** Load Data, with 1 bit per pixel. */
                virtual void LoadFromData(const unsigned char *data, const unsigned char *mask, const Math::Vector2ui &size, const Math::Vector2ui &posCenter);

            private:
                void    CreateHiddenCursor();

                Window                         *_win;
                Utils::SmartPointer< ::Cursor>  _cursor;
                static ::Cursor                 *_hiddenCursor;      // X11 doesn't provide any system to hide the cursor, so we create a transparent one
                static unsigned int             _nbCursor;
        };
    }
}

#endif
