
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

    File Created At:        22/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_WINDOW_WINDOWFRAMEBUFFER_H_
#define NC_GRAPHICS_WINDOW_WINDOW_WINDOWFRAMEBUFFER_H_

#include "../../Define.h"
#include "Window.h"
#include "../../Core/GL/FrameBuffer.h"
#include "../../Core/GL/Texture.h"

namespace Nc
{
    namespace Graphic
    {
        ///
        /**
            \todo Generate Entered/Left mouse event
            \todo Change the cursor at Entered/Left mouse event
        */
        class LGRAPHICS SubWindow : public Window
        {
            public:
                SubWindow(Window *parent, SceneGraphManager *sceneGraphManager = NULL);
                virtual ~SubWindow();

                virtual void        Create(const Math::Vector2ui &size);

                virtual void        Create(const std::string &title, const Math::Vector2ui &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel);

                virtual void        UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel);

                virtual GLContext  *CreateGLContext();

                virtual void        Close();

                /** Render off screen by using a frame buffer */
                virtual void        Render(GLContext *context);

                virtual bool        SetIcon(const Utils::FileName &)                    {return true;}

                virtual void        Resize(unsigned int width, unsigned int height);

                /** Notify a resize of the window */
                virtual void        Resized();

                /** \return a new cursor by using the parent to create it */
                virtual ICursor     *NewCursor();

                /** \return the parent of the sub window */
                Window              *Parent() const                                     {return _parent;}

                /** \return the sprite used to store the result of the off screen rendering */
                const GL::Texture   &RenderingResult() const                            {return _colorTexture;}

            protected:
                void                InitFbo();

            protected:
                Window              *_parent;           ///< parent of the sub window
                GL::FrameBuffer     _fbo;               ///< frame buffer used to render off screen
                GL::Texture		    _colorTexture;      ///< texture used to store the result of the rendering off screen

            private:
                bool                _needInitFbo;
        };
    }
}

#endif
