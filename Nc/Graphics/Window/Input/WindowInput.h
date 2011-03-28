
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

    Provide an interface for a WindowInput
    manage all the input associated to the window (keyboard/mouse/etc)

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_WINDOW_INPUT_H_
#define NC_GRAPHICS_WINDOW_WINDOW_INPUT_H_

#include <Nc/Core/Math/Math.h>
#include <Nc/Core/System/Input/Input.h>

namespace Nc
{
    namespace Graphic
    {
        class Window;

        class LCORE WindowInput : public System::Input
        {
            public:
                WindowInput(Window *win);
                virtual ~WindowInput() {};

                virtual void Create() = 0;
                virtual void CheckEvents() {};

                // generation of events
                void GenereResizeEvent(unsigned int x, unsigned int y);
                void GenereEvent(const System::Event &e);

                // return input state
                static const Math::Vector2i &MousePosition()                                {return _mousePosition;}    /// return the mouse position with the top/left ref point
                static Math::Vector2i       MousePositionInGLCoord();                                                   /// return the mouse position with the bottom/left ref point
                static bool                 KeyState(System::Key::Code code)                {return _keyStates[code];}
                static bool                 MouseButtonState(System::Mouse::Button code)    {return _mouseButtonStates[code];}

                static char                 ToChar(System::Key::Code key);

            protected:
                static bool             _keyStates[System::Key::Count];
                static bool             _mouseButtonStates[System::Mouse::Count];
                static Math::Vector2i   _mousePosition;
                bool                    _keyRepeat;
                Window                  *_win;
        };
    }
}

#endif
