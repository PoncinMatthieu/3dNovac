
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
        class LCORE GLXContext : public GLContext
        {
            public:
                GLXContext(XWindow *win/*, GLXWindow drawable = 0*/);
                virtual ~GLXContext();

                void            Create(GLContext *sharedContext = NULL);
                GLContext       *CreateNewSharedContext();

                void            Active()        {glXMakeCurrent(static_cast<XWindow*>(_win)->_display, (_pbuffer == 0) ? static_cast<XWindow*>(_win)->_xwin : _pbuffer, _context);}
                void            Disable()       {glXMakeCurrent(static_cast<XWindow*>(_win)->_display, 0, 0);}

                void            SwapBuffers()   {glXSwapBuffers(static_cast<XWindow*>(_win)->_display, (_pbuffer == 0) ? static_cast<XWindow*>(_win)->_xwin : _pbuffer);}

            private:
                ::GLXContext    _context;
                //GLXWindow       _drawable;
                GLXPbuffer      _pbuffer;  // for any shared context, we draw in a off screen renderer
        };
    }
}

#endif
