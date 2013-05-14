
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

#include "../Window/IWindow.h"
#include "../Window/SubWindow.h"
#include "../Window/ICursor.h"
#include "WindowInput.h"

using namespace Nc;
using namespace Nc::Math;
using namespace Nc::Graphic;

WindowInput::WindowInput(IWindow *win)
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
            _win->Resized();
            break;
        }

        // Mouse moved
        case System::Event::MouseMoved:
        {
            _mousePosition.data[0] = e.mouseMove.x;
            _mousePosition.data[1] = e.mouseMove.y;
            break;
        }
        case System::Event::MouseButtonPressed:
        {
            if (e.mouseButton.button == System::Mouse::Left)
                _mouseButtonLeftClock.Reset();
            else if (e.mouseButton.button == System::Mouse::Right)
                _mouseButtonRightClock.Reset();

            _mouseButtonStates[e.mouseButton.button] = true;
            _mousePosition.data[0] = e.mouseButton.x;
            _mousePosition.data[1] = e.mouseButton.y;
            break;
        }
        case System::Event::MouseButtonReleased:
        {
            _mouseButtonStates[e.mouseButton.button] = false;
            _mousePosition.data[0] = e.mouseButton.x;
            _mousePosition.data[1] = e.mouseButton.y;
            break;
        }

        case System::Event::KeyPressed:             _keyStates[e.key.code] = true;                          break;          // Key down event
        case System::Event::KeyReleased:            _keyStates[e.key.code] = false;                         break;          // Key up event

        case System::Event::MouseEntered:           _mouseIn = true;                                        break;
        case System::Event::MouseLeft:              _mouseIn = false;                                       break;

        default:
            break;
    }

    PushEvent(e);

    // forward the event to sub windows
    // and recreate some events like mouse entered/left if needed
    System::Locker l(&_win->_mutexListSubWindow);
    for (IWindow::ListSubWindow::iterator it = _win->_listSubWindow.begin(); it != _win->_listSubWindow.end(); ++it)
        ForwardEventToSubWindow(*it, e);
}

bool    WindowInput::IsDoubleClick(const System::Event &e)
{
    if (e.mouseButton.button == System::Mouse::Left)
    {
        if (_mouseButtonLeftClock.ElapsedTime() < 0.2f)
            return true;
    }
    else if (e.mouseButton.button == System::Mouse::Right)
    {
        if (_mouseButtonRightClock.ElapsedTime() < 0.2f)
            return true;
    }
    return false;
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
        {
            bool            resized = false;
            System::Event   newEvent(subWindow->Input(), e.type);
            newEvent.size.width = subWindow->Width();
            newEvent.size.height = subWindow->Height();
            if (e.size.width < newEvent.size.width)
            {
                newEvent.size.width = e.size.width;
                resized = true;
            }
            if (e.size.height < newEvent.size.height)
            {
                newEvent.size.height = e.size.height;
                resized = true;
            }
            if (resized)
                subWindow->Input()->GenereEvent(newEvent);
            break;
        }

        // forward Lost/Gained focus, TextEntered, KeyPressed, KeyReleased
        case System::Event::LostFocus:
        case System::Event::GainedFocus:
        case System::Event::TextEntered:
        case System::Event::KeyPressed:
        case System::Event::KeyReleased:
            subWindow->Input()->GenereEvent(e);
            break;

        // forward mouse wheel moved events only if the mouse is in the subwindow
        case System::Event::MouseWheelMoved:
        {
            bool buttonPressed = false;
            for (unsigned int i = 0; i < System::Mouse::Count; ++i)
            {
                if (subWindow->Input()->MouseButtonState(static_cast<System::Mouse::Button>(i)))
                    buttonPressed = true;
            }
            if (subWindow->Input()->MouseIn() || buttonPressed)
                subWindow->Input()->GenereEvent(e);
            break;
        }

        // forward mouse button events only if the mouse is in the subwindow and update the position of the mouse
        case System::Event::MouseButtonPressed:
        case System::Event::MouseButtonReleased:
        {
            System::Event newEvent(subWindow->Input(), e.type);
            newEvent.mouseButton.button = e.mouseButton.button;
            UpdateMousePositionSubWindow(subWindow, newEvent.mouseButton.x, newEvent.mouseButton.y, e.mouseButton.x, e.mouseButton.y);
            if (subWindow->Input()->MouseIn() || e.type == System::Event::MouseButtonReleased)
                subWindow->Input()->GenereEvent(newEvent);

            if (e.type == System::Event::MouseButtonReleased && (newEvent.mouseButton.x < 0 || newEvent.mouseButton.y < 0 || newEvent.mouseButton.x >= subWindow->Width() || newEvent.mouseButton.y >= subWindow->Height()))
            {
				if (_win->CurrentCursor() != NULL)
					_win->CurrentCursor()->Enable();
                System::Event newEvent(subWindow->Input(), System::Event::MouseLeft);
                subWindow->Input()->GenereEvent(newEvent);
            }
            break;
        }

        // forward mouse movement events only if the mouse in in the subwindow, and recreate a mouse Entered/Left if needed
        case System::Event::MouseMoved:
        {
            int newX, newY;
            UpdateMousePositionSubWindow(subWindow, newX, newY, e.mouseMove.x, e.mouseMove.y);

            if (!subWindow->Input()->MouseIn() && (newX > 0 && newY > 0 && newX < subWindow->Width() && newY < subWindow->Height()))
            {
				if (_win->CurrentCursor() != NULL)
	                subWindow->CurrentCursor()->Enable();
                System::Event newEvent(subWindow->Input(), System::Event::MouseEntered);
                subWindow->Input()->GenereEvent(newEvent);
            }
            else if (subWindow->Input()->MouseIn() && (newX < 0 || newY < 0 || newX >= subWindow->Width() || newY >= subWindow->Height()))
            {
				if (_win->CurrentCursor() != NULL)
					_win->CurrentCursor()->Enable();
                System::Event newEvent(subWindow->Input(), System::Event::MouseLeft);
                subWindow->Input()->GenereEvent(newEvent);
            }

            bool buttonPressed = false;
            for (unsigned int i = 0; i < System::Mouse::Count; ++i)
            {
                if (subWindow->Input()->MouseButtonState(static_cast<System::Mouse::Button>(i)))
                    buttonPressed = true;
            }

            if (subWindow->Input()->MouseIn() || buttonPressed)
            {
                System::Event newEvent(subWindow->Input(), e.type);
                newEvent.mouseMove.x = newX;
                newEvent.mouseMove.y = newY;
                subWindow->Input()->GenereEvent(newEvent);
            }
            break;
        }

        // do not forward mouse entered event
        case System::Event::MouseEntered:
            return;

        // forward mouse left
        case System::Event::MouseLeft:
            if (subWindow->Input()->MouseIn())
            {
				if (_win->CurrentCursor() != NULL)
					_win->CurrentCursor()->Enable();
                subWindow->Input()->GenereEvent(e);
            }
            break;
    }
}

void    WindowInput::UpdateMousePositionSubWindow(SubWindow *subWindow, int &newX, int &newY, const int &oldX, const int &oldY)
{
    newX = oldX - subWindow->Pos().data[0];
    newY = oldY - (_win->Height() - subWindow->Pos().data[1] - subWindow->Height());
}

Vector2i WindowInput::MousePositionInGLCoord() const
{
    return Vector2i(_mousePosition.data[0], _win->Height() - _mousePosition.data[1]);
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
