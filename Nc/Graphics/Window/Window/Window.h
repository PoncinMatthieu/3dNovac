
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
/*-----------------------------------------------------------------------------

    Provide an interface for the gestion of a window
    Provide usefull function for the managing of a window
//TODO: Actually we can't manage more than one window

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

        class LCORE Window : public Utils::NonCopyable
        {
            public:
                enum PATTERN
                {
                    Resizeable  =   1 << 0,         /// set the window resizable
                    Fullscreen  =   1 << 1,         /// fullscreen mode
                    Titlebar    =   1 << 2,         /// show the Titlebar
                    Closeable   =   1 << 3          /// set the window closable
                };

            public:
                Window() : _isCreate(false), _input(NULL), _own(false), _defaultCursor(NULL), _currentCursor(NULL)  {}
                virtual ~Window() {}

                // return true if the Window is create
                virtual bool        IsCreate() const  {return _isCreate;}

                // interface to create the window with the suitable attribute
                virtual void        Create(const std::string &title, const Math::Vector2ui &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel) = 0;
                virtual void        UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel) = 0;
                virtual GLContext  *CreateGLContext() = 0;

                // close the window
                virtual void        Close() = 0;

                // accessor
                virtual bool                    SetIcon(const Utils::FileName &f) = 0;
                virtual void                    Resize(unsigned int width, unsigned int height) = 0;
                static inline unsigned int		Width()                 {return _width;}
                static inline unsigned int		Height()                {return _height;}
                inline WindowInput              *GetInput()             {return _input;}                // get a pointer to a Windowinput who manage the inputs of a window
                inline bool                     IsOwn()                 {return _own;}
                inline unsigned int             AntialiasingLevel()     {return _antialisingLevel;}
                inline ICursor                  *DefaultCursor()        {return _defaultCursor;}
                inline ICursor                  *CurrentCursor()        {return _currentCursor;}
                virtual ICursor                 *NewCursor() = 0;

            protected:
                bool                    _isCreate;
                static unsigned int     _width;
                static unsigned int     _height;
                GLContext               *_context;
                WindowInput             *_input;
                bool                    _own;
                unsigned int            _antialisingLevel;
                ICursor                 *_defaultCursor;
                ICursor                 *_currentCursor;

                friend class WindowInput;
        };
    }
}

#endif
