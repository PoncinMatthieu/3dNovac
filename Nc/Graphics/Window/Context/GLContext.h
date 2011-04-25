
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

        /// Interface to provide an OpenGL context for multiarchi
        /**
            Typically a GLContext is created in 2 ways. With a Window by calling the method Window::CreateGLContext(),
            or with an other context by calling the method GLContext::CreateNewSharedContext().
        */
        class LGRAPHICS GLContext : public Utils::NonCopyable
        {
            public:
                GLContext(Window *win);
                virtual ~GLContext();

                /** Create the context opengl */
                virtual void        Create(GLContext *sharedContext = NULL) = 0;

                /** Create a new shared OpenGL context with the current */
                virtual GLContext  *CreateNewSharedContext() = 0;

                /** Active the opengl context */
                virtual void        Active() = 0;

                /** Disable the opengl context */
                virtual void        Disable() = 0;

                /** swap the buffers, (for a double buffered appli) */
                virtual void        SwapBuffers() = 0;

            protected:
                Window  *_win;          ///< The instance of the associated window
                bool    _isCreate;      ///< a boolean to define if the context is create
                bool    _isShared;      ///< a boolean to define if the context is shared
        };
    }
}

#endif
