
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

#ifndef NC_GRAPHIC_WINDOW_XWINDOW_H_
#define NC_GRAPHIC_WINDOW_XWINDOW_H_

#include <X11/Xlib.h>
#include "../GL/GL.h"
#include <GL/glx.h>

#include "IWindow.h"
#include "../Input/XWindowInput.h"
#include "XCursor.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of a IWindow for an X11 server.
        /**
            For more detail, please see the IWindow documentation.
        */
        class Window : public IWindow
        {
            public:
                Window(SceneGraphManager *sceneGraphManager = NULL);
                Window(const std::string &title, const Math::Vector2ui &size, const WindowStyle &style, const Utils::FileName &icone, unsigned int antialiasingLevel, SceneGraphManager *sceneGraphManager = NULL);
                virtual ~Window();

                virtual void            Create(const std::string &title, const Math::Vector2ui &size, const WindowStyle &style, const Utils::FileName &icone, unsigned int antialiasingLevel);
                virtual void            UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel);
                virtual GLContext      *CreateGLContext();
                virtual void            Close();

                virtual bool            SetIcon(const Utils::FileName &f);
                virtual void            Resize(unsigned int width, unsigned int height);

				virtual void		    Hide();
				virtual void		    Show();

                virtual ICursor         *NewCursor();


            private:
                void                    SwitchToFullscreen(const Math::Vector2ui &size);
                void                    SetWindowStyle(const WindowStyle &style);
                void                    ChooseBestVisualInfo();

                Display         *_display;
                int             _screen;
                GLXFBConfig     *_fbConfig;
                XVisualInfo     *_vInfo;
                ::Window        _xwin;

                friend class GLXContext;
                friend class XWindowInput;
                friend class Cursor;
        };
    }
}

#endif
