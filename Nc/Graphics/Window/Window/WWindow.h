
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

    File Created At:        02/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_WWINDOW_H_
#define NC_SYSTEM_WWINDOW_H_

#include "Window.h"
#include "WCursor.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of a Window for a Win32 System
        /**
            For more detail, please see the Window documentation
        */
        class LSYSTEM WWindow : public Window
        {
            public:
                WWindow();
                virtual ~WWindow();

                virtual void				Create(const std::string &title, const Vector2i &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel);
                virtual void				UseExistingWindow(void *disp, int winId, const Vector2i &size, unsigned int antialiasingLevel);
                virtual GLRenderer			*CreateRenderer();

                virtual void				Close();

				virtual bool				SetIcon(const Utils::FileName &f);
                virtual void				Resize(unsigned int width, unsigned int height);
				virtual System::ICursor		*NewCursor()										{return new System::Cursor(this);}

            protected:
				void						SwitchToFullscreen(const Vector2i &size);

                static const char *_classNameA;
                HWND          _handle;
                DWORD         _win32Style;
                HICON         _icon;

                friend class WGLRenderer;
                friend class WWindowInput;
				friend class Cursor;
        };
    }
}

#endif
