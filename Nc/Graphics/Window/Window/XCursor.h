
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
/*--------------------------------------------------------------------------------

    Provide an implementation of cursor for an X11 system
    the cursor is actually loading the data with only 1 bit per pixel
    TODO implemente cursor loading with more than 1 bit per pixel

--------------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_XCURSOR_H_
#define NC_GRAPHICS_WINDOW_XCURSOR_H_

#include <X11/Xlib.h>
#include <Nc/Core/Math/Math.h>
#include "ICursor.h"

namespace Nc
{
    namespace Graphic
    {
        class XWindow;

        class Cursor : public ICursor
        {
            public:
                Cursor(XWindow *w);
                virtual ~Cursor();

                virtual void Enable();
                virtual void Disable();

                /// Load Data, with 1 bit per pixel
                virtual void LoadFromData(const unsigned char *data, const unsigned char *mask, const Math::Vector2ui &size, const Math::Vector2ui &posCenter);
                virtual void LoadFromXpm(const char *xpm[]);

            private:
                void    CreateHiddenCursor();

                XWindow                 *_win;
                ::Cursor                *_cursor;
                static ::Cursor         *_hiddenCursor;      // X11 doesn't provide any system to hide the cursor, so we create a transparent one
                static unsigned int     _nbCursor;
        };
    }
}

#endif
