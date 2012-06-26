
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

    File Created At:        07/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_GL_WIN32_RENDERER_H_
#define NC_SYSTEM_GL_WIN32_RENDERER_H_

#include <iostream>
#include "GLContext.h"
#include "../Window/WWindow.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of GLContext for a Win32 system
        /**
            For more detail please show the class GLContext
        */
        class LGRAPHICS WGLContext : public GLContext
        {
            public:
                WGLContext(WWindow *win, HDC drawable = 0);
                virtual ~WGLContext();

                void            Create(GLContext *sharedContext = NULL);
                GLContext		*CreateNewSharedContext();

                virtual void 	Active();
                virtual void	Disable();
				virtual void	SwapBuffers();

			private:
				/** Choose the proper pixel format */
				void			ChoosePixelFormat();
				/** Set the choosed pixel format */
				void			SetPixelFormat();
				/** Create a dummy window and context to be able to load an extention */
				void			CreateDummyContext(HWND &dummyWindowHandle, HDC &dummyDrawable, HGLRC &dummyContext);
				/** Delete the dummy window and context */
				void			DeleteDummyContext(HWND dummyWindowHandle, HDC dummyDrawable, HGLRC dummyContext);

				/** Display an error window */
				void			ShowError(char *title);

            private:
                HGLRC					_context;
                HDC						_drawable;

                int						_format;
        };
    }
}

#endif
