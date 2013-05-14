/*
	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

#include "WWindowInput.h"
#include "../Window/WWindow.h"

#ifndef XBUTTON1
    #define XBUTTON1 0x0001
#endif
#ifndef XBUTTON2
    #define XBUTTON2 0x0002
#endif
#ifndef MAPVK_VK_TO_VSC
    #define MAPVK_VK_TO_VSC (0)
#endif

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;

WWindowInput::WWindowInput(Graphic::Window *win) : WindowInput(win)
{
    if (_win == NULL)
        throw Utils::Exception("WWindowInput", "Can't create input, window is null");
    _isCursorIn = false;
    _keyRepeatEnabled = true;
    _callback = NULL;
}

WWindowInput::~WWindowInput()
{
}

void WWindowInput::Create()
{
    if (!_win->IsOwn())
    {
        // We change the event procedure of the control (it is important to save the old one)
        SetWindowLongPtr(static_cast<Graphic::Window*>(_win)->_handle, GWLP_USERDATA, reinterpret_cast<long>(this));
        _callback = SetWindowLongPtr(static_cast<Graphic::Window*>(_win)->_handle, GWLP_WNDPROC, reinterpret_cast<long>(&WWindowInput::GlobalOnEvent));
    }
}

void WWindowInput::Destroy()
{
    if (_callback != NULL)
        SetWindowLongPtr(static_cast<Graphic::Window*>(_win)->_handle, GWLP_WNDPROC, _callback);
	_callback = NULL;
}

void WWindowInput::CheckEvents()
{
 // this will cause a recursive repaint event with Qt
    if (_win->IsOwn()) // process event only if its our window
    {
        MSG Message;
        while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }
}

LRESULT CALLBACK WWindowInput::GlobalOnEvent(HWND handle, UINT message, WPARAM WParam, LPARAM LParam)
{
	// Associate handle and Window instance when the creation message is received
    if (message == WM_CREATE)
    {
        // Get WWindowInput instance (it was passed as the last argument of CreateWindow)
        long This = reinterpret_cast<long>(reinterpret_cast<CREATESTRUCT*>(LParam)->lpCreateParams);

        // Set as the "user data" parameter of the window
        SetWindowLongPtrA(handle, GWLP_USERDATA, This);
    }

    // Get the WWindowInput instance corresponding to the window handle
    WWindowInput* input = reinterpret_cast<WWindowInput*>(GetWindowLongPtrA(handle, GWLP_USERDATA));

    // Forward the event to the appropriate function
    if (input)
    {
		if (input->AttachedWindow()->IsCreate() && handle && !input->ListenerListEmpty()) // don't proceed event until window is created
            input->ProcessEvent(message, WParam, LParam);

        if (input->_callback)
            return CallWindowProcA(reinterpret_cast<WNDPROC>(input->_callback), handle, message, WParam, LParam);
    }

    // We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
    if (message == WM_CLOSE)
        return 0;
    return DefWindowProcA(handle, message, WParam, LParam);
}

void WWindowInput::ProcessEvent(UINT message, WPARAM WParam, LPARAM LParam)
{
    switch (message)
    {
        // Destroy event
        case WM_DESTROY :
        {
            _win->Close();
            break;
        }

        // Set cursor event
        case WM_SETCURSOR :
        {
            // The mouse has moved, if the cursor is in our window we must refresh the cursor
            if (LOWORD(LParam) == HTCLIENT)
			{
				if (static_cast<Graphic::Window*>(_win)->_currentCursor != NULL)
					static_cast<Graphic::Window*>(_win)->_currentCursor->Enable();
			}
            break;
        }

        // Gain focus event
        case WM_SETFOCUS :
        {
            GenereEvent(Event(this, Event::GainedFocus));
            break;
        }

        // Lost focus event
        case WM_KILLFOCUS :
        {
            GenereEvent(Event(this, Event::LostFocus));
            break;
        }

        // Resize event
        case WM_SIZE :
        {
            Event e(this, Event::Resized);
            RECT Rect;
            GetClientRect(static_cast<Graphic::Window*>(_win)->_handle, &Rect);
            e.size.width = Rect.right - Rect.left;
            e.size.height = Rect.bottom - Rect.top;
            GenereEvent(e);
            break;
        }

        // Close event
        case WM_CLOSE :
        {
            GenereEvent(Event(this, Event::Closed));
            break;
        }

        // Key down event
        case WM_KEYDOWN :
        case WM_SYSKEYDOWN :
        {
            if (_keyRepeatEnabled || ((HIWORD(LParam) & KF_REPEAT) == 0))
            {
                Event e(this, Event::KeyPressed);
                e.key.code    = ConvertVirtualKey(WParam, LParam);
                e.key.alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
                e.key.control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
                e.key.shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
                GenereEvent(e);
            }
            break;
        }

        // Text event
        case WM_CHAR :
        {
            if (_keyRepeatEnabled || ((LParam & (1 << 30)) == 0))
            {
				Event e(this, Event::TextEntered);
                e.text.unicode = static_cast<UInt32>(WParam);
                GenereEvent(e);
            }
            break;
        }

        // Key up event
        case WM_KEYUP :
        case WM_SYSKEYUP :
        {
            Event e(this, Event::KeyReleased);
            e.key.code    = ConvertVirtualKey(WParam, LParam);
            e.key.alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
            e.key.control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
            e.key.shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
            GenereEvent(e);
            break;
        }

        case WM_MOUSEWHEEL:
        {
            Event e(this, Event::MouseWheelMoved);
            e.mouseWheel.delta = static_cast<short>(HIWORD(WParam)) / 120;
            GenereEvent(e);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            Event e(this, Event::MouseButtonPressed);
            e.mouseButton.button = Mouse::Left;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
	    e.mouseButton.doubled = IsDoubleClick(e);
            GenereEvent(e);
            break;
        }

        case WM_LBUTTONUP:
        {
            Event e(this, Event::MouseButtonReleased);
            e.mouseButton.button = Mouse::Left;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }

        case WM_RBUTTONDOWN:
        {
            Event e(this, Event::MouseButtonPressed);
            e.mouseButton.button = Mouse::Right;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
	    e.mouseButton.doubled = IsDoubleClick(e);
            GenereEvent(e);
            break;
        }

        case WM_RBUTTONUP:
        {
            Event e(this, Event::MouseButtonReleased);
            e.mouseButton.button = Mouse::Right;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }

        // Mouse wheel button down event
        case WM_MBUTTONDOWN:
        {
            Event e(this, Event::MouseButtonPressed);
            e.mouseButton.button = Mouse::Middle;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }

        // Mouse wheel button up event
        case WM_MBUTTONUP:
        {
            Event e(this, Event::MouseButtonReleased);
            e.mouseButton.button = Mouse::Middle;
            e.mouseButton.x      = LOWORD(LParam);
            e.mouseButton.y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }
/*
        // Mouse X button down event
        case WM_XBUTTONDOWN:
        {
            Event e(Event::MouseButtonPressed);
            e.MouseButton.Button = HIWORD(WParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            e.MouseButton.X      = LOWORD(LParam);
            e.MouseButton.Y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }

        // Mouse X button up event
        case WM_XBUTTONUP:
        {
            Event e(Event::MouseButtonReleased);
            e.MouseButton.Button = HIWORD(WParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            e.MouseButton.X      = LOWORD(LParam);
            e.MouseButton.Y      = HIWORD(LParam);
            GenereEvent(e);
            break;
        }
*/
        // Mouse move event
        case WM_MOUSEMOVE:
        {
            if (!_isCursorIn)
            {
                TRACKMOUSEEVENT MouseEvent;
                MouseEvent.cbSize    = sizeof(TRACKMOUSEEVENT);
                MouseEvent.hwndTrack = static_cast<Window*>(_win)->_handle;
                MouseEvent.dwFlags   = TME_LEAVE;
                TrackMouseEvent(&MouseEvent);

                _isCursorIn = true;

                GenereEvent(Event(this, Event::MouseEntered));
            }

            Event e(this, Event::MouseMoved);
            e.mouseMove.x = LOWORD(LParam);
            e.mouseMove.y = HIWORD(LParam);
            GenereEvent(e);
            break;
        }

        // Mouse leave event
        case WM_MOUSELEAVE:
        {
            _isCursorIn = false;
            GenereEvent(Event(this, Event::MouseLeft));
            break;
        }
    }
}

Key::Code WWindowInput::ConvertVirtualKey(WPARAM VirtualKey, LPARAM Flags)
{
    switch (VirtualKey)
    {
        // Check the scancode to distinguish between left and right shift
        case VK_SHIFT :
        {
            static UINT LShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = (Flags & (0xFF << 16)) >> 16;
            return scancode == LShift ? Key::LShift : Key::RShift;
        }

        // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;

        // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

        // Other keys are reported properly
        case VK_LWIN :       return Key::LSystem;
        case VK_RWIN :       return Key::RSystem;
        case VK_APPS :       return Key::Menu;
        case VK_OEM_1 :      return Key::SemiColon;
        case VK_OEM_2 :      return Key::Slash;
        case VK_OEM_PLUS :   return Key::Equal;
//        case VK_OEM_MINUS :  return Key::Dash;
        case VK_OEM_4 :      return Key::LBracket;
        case VK_OEM_6 :      return Key::RBracket;
        case VK_OEM_COMMA :  return Key::Comma;
        case VK_OEM_PERIOD : return Key::Period;
        case VK_OEM_7 :      return Key::Quote;
        case VK_OEM_5 :      return Key::BackSlash;
        case VK_OEM_3 :      return Key::Tilde;
        case VK_ESCAPE :     return Key::Escape;
        case VK_SPACE :      return Key::Space;
        case VK_RETURN :     return Key::Return;
        case VK_BACK :       return Key::Back;
        case VK_TAB :        return Key::Tab;
        case VK_PRIOR :      return Key::PageUp;
        case VK_NEXT :       return Key::PageDown;
        case VK_END :        return Key::End;
        case VK_HOME :       return Key::Home;
        case VK_INSERT :     return Key::Insert;
        case VK_DELETE :     return Key::Delete;
        case VK_ADD :        return Key::Add;
        case VK_SUBTRACT :   return Key::Subtract;
        case VK_MULTIPLY :   return Key::Multiply;
        case VK_DIVIDE :     return Key::Divide;
        case VK_PAUSE :      return Key::Pause;
        case VK_F1 :         return Key::F1;
        case VK_F2 :         return Key::F2;
        case VK_F3 :         return Key::F3;
        case VK_F4 :         return Key::F4;
        case VK_F5 :         return Key::F5;
        case VK_F6 :         return Key::F6;
        case VK_F7 :         return Key::F7;
        case VK_F8 :         return Key::F8;
        case VK_F9 :         return Key::F9;
        case VK_F10 :        return Key::F10;
        case VK_F11 :        return Key::F11;
        case VK_F12 :        return Key::F12;
        case VK_F13 :        return Key::F13;
        case VK_F14 :        return Key::F14;
        case VK_F15 :        return Key::F15;
        case VK_LEFT :       return Key::Left;
        case VK_RIGHT :      return Key::Right;
        case VK_UP :         return Key::Up;
        case VK_DOWN :       return Key::Down;
        case VK_NUMPAD0 :    return Key::Numpad0;
        case VK_NUMPAD1 :    return Key::Numpad1;
        case VK_NUMPAD2 :    return Key::Numpad2;
        case VK_NUMPAD3 :    return Key::Numpad3;
        case VK_NUMPAD4 :    return Key::Numpad4;
        case VK_NUMPAD5 :    return Key::Numpad5;
        case VK_NUMPAD6 :    return Key::Numpad6;
        case VK_NUMPAD7 :    return Key::Numpad7;
        case VK_NUMPAD8 :    return Key::Numpad8;
        case VK_NUMPAD9 :    return Key::Numpad9;
        case 'A' :           return Key::A;
        case 'Z' :           return Key::Z;
        case 'E' :           return Key::E;
        case 'R' :           return Key::R;
        case 'T' :           return Key::T;
        case 'Y' :           return Key::Y;
        case 'U' :           return Key::U;
        case 'I' :           return Key::I;
        case 'O' :           return Key::O;
        case 'P' :           return Key::P;
        case 'Q' :           return Key::Q;
        case 'S' :           return Key::S;
        case 'D' :           return Key::D;
        case 'F' :           return Key::F;
        case 'G' :           return Key::G;
        case 'H' :           return Key::H;
        case 'J' :           return Key::J;
        case 'K' :           return Key::K;
        case 'L' :           return Key::L;
        case 'M' :           return Key::M;
        case 'W' :           return Key::W;
        case 'X' :           return Key::X;
        case 'C' :           return Key::C;
        case 'V' :           return Key::V;
        case 'B' :           return Key::B;
        case 'N' :           return Key::N;
        case '0' :           return Key::Num0;
        case '1' :           return Key::Num1;
        case '2' :           return Key::Num2;
        case '3' :           return Key::Num3;
        case '4' :           return Key::Num4;
        case '5' :           return Key::Num5;
        case '6' :           return Key::Num6;
        case '7' :           return Key::Num7;
        case '8' :           return Key::Num8;
        case '9' :           return Key::Num9;
    }

    return Key::Code(0);
}
