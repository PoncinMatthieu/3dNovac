
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
/*-----------------------------------------------------------------------------

    Provide an interface to manage a cursor

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_ICURSOR_H_
#define NC_GRAPHICS_WINDOW_ICURSOR_H_

#include <Nc/Core/Utils/NonCopyable.h>
#include <Nc/Core/Math/Math.h>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to manipulate a Cursor.
        /**
            A cursor can be load from an xpm format and must be Enable to be seen.
            When you Enable the cursor, the current cursor of the window will be automatically set


            <div class="title">Here is an exemple of xmp cursor:</div>
 \code
    const char *StandardCamera3d::XpmHandOpen[] =
    {
        // width height num_colors chars_per_pixel
        " 16 16 3 1 ",
        // colors
        "X c #000000",
        ". c #ffffff",
        "  c None",
        // pixels
        "       XX       ",
        "   XX X..XXX    ",
        "  X..XX..X..X   ",
        "  X..XX..X..X X ",
        "   X..X..X..XX.X",
        "   X..X..X..X..X",
        " XX X.......X..X",
        "X..XX..........X",
        "X...X.........X ",
        " X............X ",
        "  X...........X ",
        "  X..........X  ",
        "   X.........X  ",
        "    X.......X   ",
        "     X......X   ",
        "     X......X   ",
        // pos mouse center
        "0,0"
    };
 \endcode
        */
        class LIB_NC_GRAPHICS ICursor
        {
            public:
                ICursor(IWindow *attachedWindow) : _attachedWindow(attachedWindow)   {}
                virtual ~ICursor()                  {}

                /** \return a clone of the current cursor. */
                virtual ICursor *Clone() const = 0;

                /** Enable the cursor and set the current cursor of the attached window. */
                virtual void    Enable() = 0;
                /** Disable the cursor. */
                virtual void    Disable() = 0;

                /** Load the cursor from an Xpm image. */
                virtual void    LoadFromXpm(const char *xpm[]) = 0;

                //virtual void LoadFromImage(const Utils::FileName &f) = 0; //todo

                /** load data from 1 bit per pixel. */
                virtual void    LoadFromData(const unsigned char *data, const unsigned char *mask, const Math::Vector2ui &size, const Math::Vector2ui &posCenter) = 0;

                /** Set the attached window, can be call to attach the cursor to a sub window .*/
                void            AttachWindow(IWindow *attachedWindow)    {_attachedWindow = attachedWindow;}

            protected:
                void            SetCurrentCursor(ICursor *cur)
                {
                    _attachedWindow->_currentCursor = cur;
                }

            protected:
                IWindow          *_attachedWindow;       ///< instance of the attached window.
        };
    }
}

#endif
