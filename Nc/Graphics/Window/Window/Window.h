
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

#ifndef NC_GRAPHICS_WINDOW_WINDOW_H_
#define NC_GRAPHICS_WINDOW_WINDOW_H_

#include <iostream>
#include <Nc/Core/Math/Math.h>
#include "../../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class GLContext;
        class WindowInput;
        class ICursor;
        class SubWindow;

        /// Interface to create and manage a window for multiarchi
        /**
            \todo Actually we can't manage more than one window
        */
        class LGRAPHICS Window : public Utils::NonCopyable
        {
            public:
                enum PATTERN
                {
                    Resizeable  =   1 << 0,         ///< set the window resizable
                    Fullscreen  =   1 << 1,         ///< fullscreen mode
                    Titlebar    =   1 << 2,         ///< show the Titlebar
                    Closeable   =   1 << 3          ///< set the window closable
                };

                typedef std::list<SubWindow*>       ListSubWindow;

            public:
                Window();
                virtual ~Window();

                /** Return true if the Window is create */
                virtual bool        IsCreate() const  {return _isCreate;}

                /** Interface to create the window with the suitable attributes */
                virtual void        Create(const std::string &title, const Math::Vector2ui &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel) = 0;

                /** Interface to create a Window instance using an existing window (for Qt by exemple) */
                virtual void        UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel) = 0;

                /** Create and return a new GLContext associated to the window */
                virtual GLContext  *CreateGLContext() = 0;

                /** close the window */
                virtual void        Close() = 0;

                // accessor
                /** Set an Icon to the window with the given image filename */
                virtual bool                    SetIcon(const Utils::FileName &f) = 0;

                /** Resize the window */
                virtual void                    Resize(unsigned int width, unsigned int height) = 0;

                /** \return the width of the window */
                inline unsigned int		        Width() const           {return _width;}

                /** \return the height of the window */
                inline unsigned int		        Height() const          {return _height;}

                /** Set the window size */
                inline void                     ResetSize(unsigned int width, unsigned int height)      {_width = width; _height = height;}

                /** Return the OpenGL context associated */
                inline GLContext                *GetContext()           {return _context;}

                /** Return the window input associated */
                inline WindowInput              *GetInput()             {return _input;}

                /** return false if the window use an existing window (Created with the method UseExistingWindow) */
                inline bool                     IsOwn()                 {return _own;}

                /** Return the antialiasing level available, this could be smaller than the level you put in the Create method */
                inline unsigned int             AntialiasingLevel()     {return _antialiasingLevel;}

				/** Return the number of depth bits */
                inline unsigned int             Depth()					{return _depth;}

				/** Return the number of stencil bits  */
                inline unsigned int             Stencil()				{return _stencil;}

				/** Return the number of bits per pixel (color buffer) */
                inline unsigned int             BitsPerPixel()			{return _bitsPerPixel;}

                /** Return the default cursor */
                inline ICursor                  *DefaultCursor()        {return _defaultCursor;}

                /** Return the activated cursor */
                inline ICursor                  *CurrentCursor()        {return _currentCursor;}

                /** create and return a New Cursor */
                virtual ICursor                 *NewCursor() = 0;

            protected:
                ListSubWindow           _listSubWindow;     ///< a list of subwindow, which will be used for exemple to forward inputs to a subwindow

                bool                    _isCreate;          ///< true if the window has been created
                unsigned int            _width;             ///< the width of the window
                unsigned int            _height;            ///< the height of the window
                GLContext               *_context;          ///< the associated OpenGL context
                WindowInput             *_input;            ///< the window input associated
                bool                    _own;               ///< false if the window use an existing window (Created with the method UseExistingWindow)
                unsigned int            _antialiasingLevel; ///< the antialising level of the window
                unsigned int			_depth;				///< number of bits of the depth buffer
                unsigned int			_stencil;			///< number of bits of the pixel buffer
                unsigned int			_bitsPerPixel;		///< number of bits of the color buffer
                ICursor                 *_defaultCursor;    ///< instance of the default cursor of the window
                ICursor                 *_currentCursor;    ///< instance of the current cursor activated

                friend class WindowInput;
                friend class SubWindow;
        };
    }
}

#endif
