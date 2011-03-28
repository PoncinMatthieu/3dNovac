
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

    Provide a GLContext interface for multiarchi

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_CONTEXT_GLCONTEXT_H_
#define NC_GRAPHICS_WINDOW_CONTEXT_GLCONTEXT_H_

#include <iostream>
#include <list>
#include <map>
#include <Nc/Core/System/API/API.h>
#include "../../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class Window;

        class LCORE GLContext : public Utils::NonCopyable
        {
            public:
                GLContext(Window *win);
                virtual ~GLContext();

                // return the current Renderer Associate to the current thread
                static GLContext *Current();

                // create the context opengl
                virtual void        Create(GLContext *sharedContext = NULL) = 0;
                virtual GLContext  *CreateNewSharedContext() = 0;

                // active/disable the opengl context
                virtual void        Active() = 0;
                virtual void        Disable() = 0;

                // swap the buffers, (for a double buffered appli)
                void                Refresh();  // swap buffers and limit the frame rate
                virtual void        SwapBuffers() = 0;

            protected:
                Window  *_win;
                bool    _isCreate;
                bool    _isShared;
        };
    }
}

#endif
