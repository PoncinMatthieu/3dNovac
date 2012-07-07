
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

#ifndef NC_SYSTEM_XCURSOR_H_
#define NC_SYSTEM_XCURSOR_H_

#include "../../Define.h"
#include "ICursor.h"

namespace Nc
{
    namespace Graphic
    {
        class WWindow;

        /// Implementation of a ICursor for a Win32 system
        /**
            \warning TODO: Cursor loading is not implemented yet :(

            The cursor is actually loading the data with only 1 bit per pixel <br/>
            To create a cursor you will need to have an XWindow but you shouldn't create a create directly a cursor, the best way is to use the methode Window::NewCursor() <br/>
            <br/>
        */
        class LGRAPHICS Cursor : public ICursor
        {
            public:
                Cursor(WWindow *w);
                virtual ~Cursor();

                /** \return a clone of the current cursor */
				virtual ICursor *Clone() const		{return new Cursor(*this);}

				/** Enable the cursor */
                virtual void Enable();

                /** Disable the cursor */
                virtual void Disable();

                /** Load from an Xpm (for more detail show the detailed description of ICursor) */
                virtual void LoadFromXpm(const char *xpm[]);

                virtual void LoadFromData(const unsigned char *data, const unsigned char *mask, const Vector2ui &size, const Vector2ui &posCenter);

            private:
                void    CreateHiddenCursor();

                WWindow                 *_win;
                HCURSOR					_cursor;
        };
    }
}

#endif
