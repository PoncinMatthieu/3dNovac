
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

#ifndef NC_GRAPHICS_WINDOW_SUBWINDOW_H_
#define NC_GRAPHICS_WINDOW_SUBWINDOW_H_

#include "../Define.h"
#include "IWindow.h"
#include "../GL/FrameBuffer.h"
#include "../GL/Texture.h"

namespace Nc
{
    namespace Graphic
    {
        /// To manage a sub window.
        /**
            The SubWindow render it's SceneGraphManager offscreen on a texture by using a framebuffer.
            Then the texture can be accessed with the method 'RenderingResult' and be used or rendered afterward.

            A SubWindow will usually be used with a GUI::SubWindow so the sub window can be rendered into the GUI of your application.

            \sa
                - IWindow
                - GUI::SubWindow

            \todo Generate Entered/Left mouse event.
            \todo Change the cursor at Entered/Left mouse event.
        */
        class LIB_NC_GRAPHICS SubWindow : public IWindow
        {
            public:
                SubWindow(IWindow *parent, SceneGraphManager *sceneGraphManager = NULL);
                virtual ~SubWindow();

                /** Create the sub window by defining it's initial size. */
                virtual void        Create(const Math::Vector2ui &size);
                /** Create the sub window by defining it's initial size. Every other parameters are ignored. */
                virtual void        Create(const std::string &title, const Math::Vector2ui &size, const Utils::Mask<Style> &style, const Utils::FileName &icon, unsigned int antialiasingLevel);
                /** Create the sub window by defining it's initial size. Every other parameters are ignored. */
                virtual void        UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel);

                /** Create and return a valid GLContext by using the parent window method CreateGLContext. */
                virtual GLContext  *CreateGLContext();

                /** Close the window, remove the instance of the window from the parent window. */
                virtual void        Close();

                /** Render off screen by using a frame buffer. */
                virtual void        Render(GLContext *context);

                /** Set the postion of the subwindow into the parent. */
                void                Pos(const Vector2i &pos)                            {_pos = pos;}
                /** \return the postion of the subwindow into the parent. */
                const Vector2i      &Pos() const                                        {return _pos;}

				/** Hide the window. */
				virtual void		Hide();
				/** Show the window. */
				virtual void		Show();

                /** \return true and do nothing. SubWindows don't have icon support. */
                virtual bool        SetIcon(const Utils::FileName &)                    {return true;}

                /** Set the size of the window and call the method 'Resized'. */
                virtual void        Resize(unsigned int width, unsigned int height);

                /** Notify a resize of the window. Will ask to initialize the fbo at the next render pass. */
                virtual void        Resized();

                /** \return a new cursor by using the parent to create it. */
                virtual ICursor     *NewCursor();

                /** \return the parent of the sub window. */
                IWindow              *Parent() const                                     {return _parent;}

                /** \return the sprite used to store the result of the off screen rendering. */
                const GL::Texture   &RenderingResult() const                            {return _colorTexture;}

            protected:
                void                InitFbo();

            protected:
                IWindow              *_parent;           ///< parent of the sub window.
                Vector2i            _pos;               ///< position relative to the parent.
                GL::FrameBuffer     _fbo;               ///< frame buffer used to render off screen.
                GL::Texture		    _colorTexture;      ///< texture used to store the result of the rendering off screen.

            private:
                bool                _needInitFbo;
        };
    }
}

#endif
