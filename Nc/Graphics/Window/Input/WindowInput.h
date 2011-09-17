
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

#ifndef NC_GRAPHICS_WINDOW_WINDOW_INPUT_H_
#define NC_GRAPHICS_WINDOW_WINDOW_INPUT_H_

#include <Nc/Core/Math/Math.h>
#include <Nc/Core/System/Input/Input.h>
#include "../../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class Window;

        /// Interface to manage the window inputs (keyboard/mouse/etc)
        /**
            Manage all the input associated to the window <br/>
            A WindowInput is automatically created with his associated window so to get it call Window::GetInput()

            \sa
                - Window
                - GLContext
        */
        class LGRAPHICS WindowInput : public System::Input
        {
            public:
                WindowInput(Window *win);
                virtual ~WindowInput() {};

                /** Create and init the input */
                virtual void Create() = 0;
                /** Check the events of the input */
				virtual void CheckEvents(){};

                /** Generate a resize event */
                void GenereResizeEvent(unsigned int x, unsigned int y);

                /** Generate an event */
                void GenereEvent(const System::Event &e);

                // return input state
                /** \return the mouse position with the top/left ref point */
                static const Math::Vector2i &MousePosition()                                {return _mousePosition;}

                /** \return the mouse position with the bottom/left ref point */
                static Math::Vector2i       MousePositionInGLCoord();

                /** \return true if the key is enabled */
                static bool                 KeyState(System::Key::Code code)                {return _keyStates[code];}

                /** \return true if the mouse button is enabled */
                static bool                 MouseButtonState(System::Mouse::Button code)    {return _mouseButtonStates[code];}

                /** \return the corresponding char */
                static char                 ToChar(System::Key::Code key);

            protected:
                static bool             _keyStates[System::Key::Count];             ///< Store the key states
                static bool             _mouseButtonStates[System::Mouse::Count];   ///< Store the mouse button states
                static Math::Vector2i   _mousePosition;                             ///< Store the current position of the mouse
                bool                    _keyRepeat;                                 ///< true if the key are repeating
                Window                  *_win;                                      ///< instance to the attached windows
        };
    }
}

#endif
