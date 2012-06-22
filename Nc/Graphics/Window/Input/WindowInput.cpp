
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

#include "../Window/Window.h"
#include "../Window/SubWindow.h"
#include "WindowInput.h"

using namespace Nc;
using namespace Nc::Math;
using namespace Nc::Graphic;

WindowInput::WindowInput(Window *win)
    : _win(win)
{
    _keyRepeat = true;
    _mouseIn = true;
    int i;
    for (i = 0; i < System::Key::Count; ++i)
        _keyStates[i] = false;
    for (i = 0; i < System::Mouse::Count; ++i)
        _mouseButtonStates[i] = false;
}

WindowInput::~WindowInput()
{

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

        case System::Event::KeyPressed:             _keyStates[e.key.code] = true;                          break;          // Key down event
        case System::Event::KeyReleased:            _keyStates[e.key.code] = false;                         break;          // Key up event
        case System::Event::MouseButtonPressed:     _mouseButtonStates[e.mouseButton.button] = true;        break;          // Mouse button pressed
        case System::Event::MouseButtonReleased:    _mouseButtonStates[e.mouseButton.button] = false;       break;          // Mouse button released

        case System::Event::MouseEntered:           _mouseIn = true;                                        break;
        case System::Event::MouseLeft:              _mouseIn = false;                                       break;

        default:
            break;
    }

    PushEvent(e);

    // forward the event to sub windows
    // and recreate some events like mouse entered/left if needed
    for (Window::ListSubWindow::iterator it = _win->_listSubWindow.begin(); it != _win->_listSubWindow.end(); ++it)
        ForwardEventToSubWindow(*it, e);
}

void    WindowInput::ForwardEventToSubWindow(SubWindow *subWindow, const System::Event &e)
{
    switch (e.type)
    {
        // do not forward Closed event
        case System::Event::Closed:
            return;

        // resize the subwindow if the resize value is smaller than the subwindow
        case System::Event::Resized:
            if (e.size.width < subWindow->Width() || e.size.height < subWindow->Height())
                subWindow->GetInput()->GenereEvent(e);
            break;

        // forward Lost/Gained focus, TextEntered, KeyPressed, KeyReleased
        case System::Event::LostFocus:
        case System::Event::GainedFocus:
        case System::Event::TextEntered:
        case System::Event::KeyPressed:
        case System::Event::KeyReleased:
            subWindow->GetInput()->GenereEvent(e);
            break;

        // forward mouse button events only if the mouse is in the subwindow
        case System::Event::MouseWheelMoved:
        case System::Event::MouseButtonPressed:
        case System::Event::MouseButtonReleased:
            if (subWindow->GetInput()->MouseIn())
                subWindow->GetInput()->GenereEvent(e);
            break;

        // forward mouse movement events only if the mouse in in the subwindow, and recreate a mouse Entered/Left if needed
        case System::Event::MouseMoved:

            /// \todo regenerate Mouter Entered/Left events when needed

            if (subWindow->GetInput()->MouseIn())
                subWindow->GetInput()->GenereEvent(e);
            return;

        // do not forward mouse entered event
        case System::Event::MouseEntered:
            return;

        // forward mouse left
        case System::Event::MouseLeft:
            subWindow->GetInput()->GenereEvent(e);
            break;
    }
}

Vector2i WindowInput::MousePositionInGLCoord() const
{
    return Vector2i(_mousePosition.Data[0], _win->Height() - _mousePosition.Data[1]);
}

char WindowInput::ToChar(System::Key::Code key) const
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
