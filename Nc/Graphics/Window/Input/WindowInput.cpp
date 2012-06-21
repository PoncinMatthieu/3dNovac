
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "WindowInput.h"
#include "../Window/Window.h"

using namespace Nc;
using namespace Nc::Math;
using namespace Nc::Graphic;

bool        WindowInput::_keyStates[System::Key::Count];
bool        WindowInput::_mouseButtonStates[System::Mouse::Count];

WindowInput::WindowInput(Window *win) : _win(win)
{
    _keyRepeat = true;
    int i;
    for (i = 0; i < System::Key::Count; ++i)
        _keyStates[i] = false;
    for (i = 0; i < System::Mouse::Count; ++i)
        _mouseButtonStates[i] = false;
}

void WindowInput::GenereResizeEvent(unsigned int x, unsigned int y)
{
    System::Event   e(this, System::Event::Resized);
    e.size.width = x;
    e.size.height = y;
    GenereEvent(e);
}

void WindowInput::GenereEvent(const System::Event &e)
{
    switch (e.type)
    {
        // Resize event
        case System::Event::Resized:
        {
            _win->_width = e.size.width;
            _win->_height = e.size.height;
            break;
        }

        // Mouse moved
        case System::Event::MouseMoved:
        {
            _mousePosition.Data[0] = e.mouseMove.x;
            _mousePosition.Data[1] = e.mouseMove.y;
            break;
        }

        case System::Event::KeyPressed:             _keyStates[e.key.code] = true;                          break;        // Key down event
        case System::Event::KeyReleased:            _keyStates[e.key.code] = false;                         break;        // Key up event
        case System::Event::MouseButtonPressed:     _mouseButtonStates[e.mouseButton.button] = true;        break;        // Mouse button pressed
        case System::Event::MouseButtonReleased:    _mouseButtonStates[e.mouseButton.button] = false;       break;        // Mouse button released
        default:
            break;
    }

    PushEvent(e);
}

Vector2i WindowInput::MousePositionInGLCoord()
{
    return Vector2i(_mousePosition.Data[0], _win->Height() - _mousePosition.Data[1]);
}

char WindowInput::ToChar(System::Key::Code key)
{
    char c = (char)key;
    if (KeyState(System::Key::LShift) || KeyState(System::Key::RShift))
    {
        if (key == System::Key::SemiColon)
            c = ':';
        else if (key == System::Key::Add)
            c = '+';
        else if (key == System::Key::Underscore)
            c = '_';
        else if (c >= 'a' && c <= 'z') // Maj
            c -= ('a' - 'A');
        else if (key == System::Key::Sup)
            c = '>';
        else if (key == System::Key::Inf)
            c = '<';
        else if (key == System::Key::Pipe)
            c = '|';
        else
            c = '\0';
    }
    else if (key == System::Key::Space)
        c = ' ';
    else if (key == System::Key::Tab)
        c = '\t';
    else if (key == System::Key::Return)
        c = '\n';
    else if (key == System::Key::Equal)
        c = '=';
    else if (key == System::Key::Period) //dot
        c = '.';
    else if (key == System::Key::Subtract)
        c = '-';
    else if (key == System::Key::Slash || key == System::Key::Divide)
        c = '/';
    else if (key == System::Key::BackSlash)
        c = '\\';
    else if (key == System::Key::SemiColon)
        c = ';';
    else if (c < ' ' || c > '~')
        c = '\0';
    return c;
}
