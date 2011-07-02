
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
/*-----------------------------------------------------------------------------

    Provide an implementation of GLContext for a server X

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_CONTEXT_GLXCONTEXT_H_
#define NC_GRAPHICS_WINDOW_CONTEXT_GLXCONTEXT_H_

#include <iostream>
#include "GLContext.h"
#include "../Window/XWindow.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of GLContext for an X11 system
        /**
            \sa
                - GLContext
        */
        class LCORE GLXContext : public GLContext
        {
            public:
                GLXContext(XWindow *win);
                virtual ~GLXContext();

                virtual void        Create(GLContext *sharedContext = NULL);
                virtual GLContext   *CreateNewSharedContext();

                virtual void        Active()
                {
                    if (glXMakeCurrent(_display, static_cast<XWindow*>(_win)->_xwin, _context) == 0)
                        LOG_ERROR << "Make current failed" << std::endl;
                }

                virtual void        Disable()
                {
                    if (glXMakeCurrent(_display, 0, 0) == 0)
                        LOG_ERROR << "Make current failed" << std::endl;
                }

                virtual void        SwapBuffers()   {glXSwapBuffers(_display, static_cast<XWindow*>(_win)->_xwin);}

            private:
                Display         *_display;      // the display connection to the X server of the context (A shared context recreate a display connection to the Xserver and a context associated to a window use the display connection of the window)
                ::GLXContext    _context;
        };
    }
}

#endif
